// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.cpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:41:07 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 01:12:21 by tmielcza         ###   ########.fr       //
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

ParticleSystem::ParticleSystem(GPUContext &context, int size, std::string source) :
	glBuff(size)
{
	cl_int		err;
	std::string	err_str;
	float		buf[3 * 10];

	this->clBuff = new cl::BufferGL(context.getCLContext(), CL_MEM_READ_WRITE,
									this->glBuff.getId(), &err);
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
	this->kernel = cl::Kernel(this->program, "add", &err);
	CheckCLErrorStatus(err, "Can't get CL Kernel.");

	cl::Event event;

	auto add = cl::make_kernel<cl::BufferGL>(this->kernel);
	event = add(cl::EnqueueArgs(this->queue, cl::NDRange(size)), *this->clBuff);
	event.wait();

	err = this->queue.enqueueReadBuffer(*this->clBuff, false, 0, sizeof(buf), buf, NULL, &event);
	CheckCLErrorStatus(err, "Can't read buffer.");
	event.wait();
	err = this->queue.flush();
	for (int i = 0; i < 10; i++)
		printf("%f %f %f\n", buf[3 * i], buf[3 * i + 1], buf[3 * i + 2]);
	this->ComputeParticles();
}

void		ParticleSystem::ComputeParticles(void)
{
//	glFlush();

//	cl_int						err;
	std::vector<cl::Memory>		test = {*this->clBuff};

	this->queue.enqueueAcquireGLObjects(&test, NULL, NULL);

	// DO SOME SHIT

	this->queue.enqueueReleaseGLObjects(&test, NULL, NULL);
	this->queue.flush();
}

void		ParticleSystem::RenderParticles(void)
{
	
}

ParticleSystem::~ParticleSystem()
{
	delete this->clBuff;
}
