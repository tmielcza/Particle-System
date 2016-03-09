// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:41:07 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/09 05:34:12 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <vector>
#include "ParticleSystem.hpp"

inline void CheckCLErrorStatus(cl_int error, std::string error_msg)
{
	if (error != CL_SUCCESS)
	{
		fprintf(stderr, "CL Error %d: %s\n", error, error_msg.c_str());
		throw std::exception();
	}
}

ParticleSystem::ParticleSystem(GPUContext &context, int size) :
	glBuff(GLBuffer::CreateGLBuffer<float>(4, size)),
	glBuffVelocities(GLBuffer::CreateGLBuffer<float>(4, size)),
	context(context),
	size(size),
	vao(new GLVAO(size, this->glBuff)),
	gravityCenter(0, 0, 0, 1),
	run(false)
{
	if (size <= 0 || size > 3e6)
	{
		throw std::runtime_error("Wrong particle number.");
	}
	std::string source = this->ReadFile("resources/particles.cl");

	cl_int		err;
	std::string	err_str;

	this->clBuff = new cl::BufferGL(context.getCLContext(), CL_MEM_READ_WRITE,
									this->glBuff->getId(), &err);
	CheckCLErrorStatus(err, "Can't create CL BufferGL.");
	this->clBuffVelocities = new cl::BufferGL(context.getCLContext(), CL_MEM_READ_WRITE,
									this->glBuffVelocities->getId(), &err);
	CheckCLErrorStatus(err, "Can't create CL BufferGL.");
	this->queue = cl::CommandQueue(context.getCLContext(), context.getCLDevice(),
								   0, &err);
	CheckCLErrorStatus(err, "Can't create CL CommandQueue.");
	this->program = cl::Program(context.getCLContext(), source, false, &err);
	CheckCLErrorStatus(err, "Can't create CL Program.");
	err = this->program.build(context.getCLDevices());
	if (err != CL_SUCCESS)
	{
		this->program.getBuildInfo(context.getCLDevice(), CL_PROGRAM_BUILD_LOG, &err_str);
		printf("Build error:\n%s", err_str.c_str());
		throw std::exception();
	}
	this->kernel = cl::Kernel(this->program, "sphere", &err);
	this->kernelInitCube = cl::Kernel(this->program, "cube", &err);
	CheckCLErrorStatus(err, "Can't get CL Kernel.");

	std::string vert = this->ReadFile("resources/point_vert.gl");
	std::string frag = this->ReadFile("resources/point_frag.gl");

	this->glProgram = new GLProgram(vert, frag);

	Initialize();
}

void		ParticleSystem::Initialize(void)
{
	std::vector<cl::Memory>		test = {*this->clBuff};
	cl::Event					event;

	this->queue.enqueueAcquireGLObjects(&test, NULL, NULL);
	auto init = cl::make_kernel<cl_int, cl::BufferGL>(this->kernel);
	event = init(cl::EnqueueArgs(this->queue, cl::NDRange(this->size)), this->size, *this->clBuff);
	event.wait();
	this->queue.enqueueReleaseGLObjects(&test, NULL, NULL);
	this->queue.flush();
}

void		ParticleSystem::ComputeParticles(void)
{
//	cl_int						err;
	std::vector<cl::Memory>		test = {*this->clBuff, *this->clBuffVelocities};

	if (this->run == false)
		return ;
	this->queue.enqueueAcquireGLObjects(&test, NULL, NULL);

	// DO SOME SHIT
	cl::Event	event;

	cl_float4	f {{this->gravityCenter.x, this->gravityCenter.y, this->gravityCenter.z, 1.0}};
//	cl_float4	f = *(cl_float4 *)(&this->gravityCenter);
	auto update = cl::make_kernel<cl_int, cl::BufferGL, cl::BufferGL, cl_float4>(cl::Kernel(this->program,"update"));
	event = update(cl::EnqueueArgs(this->queue, cl::NDRange(this->size)), this->size, *this->clBuff, *this->clBuffVelocities, f);
	event.wait();
	this->queue.enqueueReleaseGLObjects(&test, NULL, NULL);
	this->queue.flush();
}

void		ParticleSystem::RenderParticles(void)
{
	// A foutre ailleurs !!
	glClearColor(0, 0, 0, 0.002f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPointSize(1.0f);

	this->vao->BindWithProgram(*this->glProgram, "pos");
	this->glProgram->SetParam<float>("gcenter", (float *)&this->gravityCenter, 4);
	this->vao->Draw(*this->glProgram);
	glfwSwapBuffers(this->context.getGLFWContext());
	glFlush();
}

void		ParticleSystem::SetGravityCenter(float x, float y, float z)
{
	this->gravityCenter = Vector4(x, y, z, 1.0f);
}

std::string	ParticleSystem::ReadFile(std::string name)
{
	std::ifstream		file(name);
	std::stringstream	ss;

	ss << file.rdbuf();
	return (ss.str());
}

ParticleSystem::~ParticleSystem()
{
	delete this->clBuff;
}

void		ParticleSystem::OnOff(void)
{
	this->run = !this->run;
}
