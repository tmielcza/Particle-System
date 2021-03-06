// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:41:07 by tmielcza          #+#    #+#             //
//   Updated: 2016/05/29 17:30:18 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <chrono>
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
	run(false),
	hasGravity(true),
	// Huh.
	camera(Matrix<4,4>::Perspective(30.f * 3.14 / 180.f, context.getX() / context.getY(), 0.f, 1000.f),
		   Vector<3>(0.f, 0.f, -10.f))
{
	if (size <= 0 || size > 3e6)
	{
		throw std::runtime_error("Wrong particle number.");
	}

	std::string source = this->ReadFile("resources/particles.cl");
	std::string vert = this->ReadFile("resources/point_vert.gl");
	std::string frag = this->ReadFile("resources/point_frag.gl");
	std::string geo = this->ReadFile("resources/point_geo.gl");
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

	this->glProgram = new GLProgram(vert, frag, geo);

	Initialize(&this->kernelInitCube);
}

void		ParticleSystem::Initialize(cl::Kernel *ker)
{
	std::vector<cl::Memory>		test = {*this->clBuff};
	cl::Event					event;

	this->queue.enqueueAcquireGLObjects(&test, NULL, NULL);
	auto init = cl::make_kernel<cl_int, cl::BufferGL>(*ker);
	event = init(cl::EnqueueArgs(this->queue, cl::NDRange(this->size)), this->size, *this->clBuff);
	event.wait();
	this->queue.enqueueReleaseGLObjects(&test, NULL, NULL);
	this->queue.flush();
}

void		ParticleSystem::ComputeParticles(void)
{
	std::vector<cl::Memory>		test = {*this->clBuff, *this->clBuffVelocities};
	cl::Event	event;
	cl_float4	f {{this->gravityCenter.x, this->gravityCenter.y, this->gravityCenter.z, 1.0}};

	if (this->run == false)
		return ;
	this->queue.enqueueAcquireGLObjects(&test, NULL, NULL);
	// Chopper les kernels a l'avance plz
	if (this->hasGravity) {
		auto update = cl::make_kernel<cl_int, cl::BufferGL, cl::BufferGL, cl_float4>(cl::Kernel(this->program,"update"));
		event = update(cl::EnqueueArgs(this->queue, cl::NDRange(this->size)), this->size, *this->clBuff, *this->clBuffVelocities, f);
	} else {
		auto update = cl::make_kernel<cl_int, cl::BufferGL, cl::BufferGL>(cl::Kernel(this->program, "update_no_g"));
		event = update(cl::EnqueueArgs(this->queue, cl::NDRange(this->size)), this->size, *this->clBuff, *this->clBuffVelocities);
	}
	event.wait();

	/*
	std::vector<cl_float4>	buff(this->size);
	std::vector<cl_float4>	buffVeloc(this->size);
	cl::copy(this->queue, *this->clBuff, buff.begin(), buff.end());
	cl::copy(this->queue, *this->clBuffVelocities, buffVeloc.begin(), buffVeloc.end());
	this->SortParticles(buff, buffVeloc);
	cl::copy(this->queue, buff.begin(), buff.end(), *this->clBuff);
	cl::copy(this->queue, buffVeloc.begin(), buffVeloc.end(), *this->clBuffVelocities);
//	*/

	this->queue.enqueueReleaseGLObjects(&test, NULL, NULL);
	this->queue.flush();
}

void		ParticleSystem::RenderParticles(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->glProgram->SetParam("proj", this->camera.GetMatrix());
	this->vao->BindWithProgram(*this->glProgram, "pos");
	this->glProgram->SetParam<float>("gcenter", (float *)&this->gravityCenter, 4);
	this->glProgram->SetParam<float>("campos", (float *)&this->camera.GetPosition(), 3);
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(this->clock.now().time_since_epoch()).count();
//	printf("GROSSE BITE: %f\n", time);
	this->glProgram->SetParam<float>("time", &time, 1);
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

void		ParticleSystem::ChangeInitForm(void)
{
	const std::vector<cl::Kernel *>		initKernels = {
		&this->kernel, &this->kernelInitCube
	};

	this->currentInitKernelId = (this->currentInitKernelId + 1) % initKernels.size();
	this->Initialize(initKernels[this->currentInitKernelId]);
}

void		ParticleSystem::SetGravity(bool position)
{
	this->hasGravity = position;
}

float		ParticleSystem::DeltaTime(void)
{
	static auto				last = std::chrono::system_clock::now();;
	auto					time = std::chrono::system_clock::now();;
	float					tmp;

	tmp = std::chrono::duration_cast<std::chrono::microseconds>(time - last).count() / 1e6f;
	last = time;
	return (tmp);
}

void		ParticleSystem::Move(Vector<3> dir)
{
	this->camera.Move(dir);
}

void		ParticleSystem::CameraPitch(float a)
{
	this->camera.Pitch(a);
}

void		ParticleSystem::CameraYaw(float a)
{
	this->camera.Yaw(a);
}

template <typename T, typename Iterator>
void		RadixSort(Iterator begin, Iterator end)
{
	auto tmp = std::vector<T>(begin, end);
	auto tmp_dst = std::vector<T>(std::distance(begin, end));
	auto offsets = std::vector<size_t>(257, 0);

	for (auto byte = 0; byte < 4; byte++)
	{
		auto offset = byte * 8;
		for (auto & e : tmp) {
			offsets[((e.GetKey() >> offset) & 0xFF) + 1]++;
		}
		for (auto i = 1u; i < offsets.size(); i++) {
			offsets[i] += offsets[i - 1];
		}
		for (auto i = 0u; i < tmp.size(); i++) {
			auto bits = (tmp[i].GetKey() >> offset) & 0xFF;
			tmp_dst[offsets[bits]++] = tmp[i];
		}
		tmp.swap(tmp_dst);
		offsets.assign(offsets.size(), 0);
	}
	for (auto i = 0u; i < tmp.size(); i++) {
		*(begin++) = tmp[i];
	}
}

void		ParticleSystem::SortParticles(
				std::vector<cl_float4>& buff,
				std::vector<cl_float4>& buffVeloc)
{
	struct SortableParticle
	{
		int32_t		distance;
		size_t		id;

		size_t		GetKey(void) { return this->distance; }
		size_t		GetId(void) { return this->id; }

		SortableParticle(int32_t dist, size_t id) : distance(dist), id(id) {}
		SortableParticle(void) {}
	};

	std::vector<cl_float4>			tmp_buff(buff.size());
	std::vector<cl_float4>			tmp_buff2(buff.size());
	std::vector<SortableParticle>	to_sort(buff.size());

	for (auto i = 0u; i < buff.size(); i++) {
		auto vec = *(Vector<3> *)&buff[i] - this->camera.GetPosition();
		auto dist = vec.Length();
		dist = dist * 0xFFFFFFFF / 1000.0f;
		dist = 0xFFFFFFFF - dist;
		to_sort[i] = SortableParticle((int32_t)dist, i);
	}

	RadixSort<SortableParticle>(to_sort.begin(), to_sort.end());

	for (auto i = 0u; i < buff.size(); i++) {
		tmp_buff[to_sort[i].id] = buff[i];
		tmp_buff2[to_sort[i].id] = buffVeloc[i];
	}

	buff.swap(tmp_buff);
	buffVeloc.swap(tmp_buff2);
//	buff.assign(tmp_buff.begin(), tmp_buff.end());
}
