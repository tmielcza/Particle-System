// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GPUContext.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 22:33:08 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/10 01:41:53 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "GPUContext.hpp"

void		GPUContext::GetCLDevices(void)
{
	std::vector<cl::Platform>			platforms;
	cl_int								errcode;
	std::string							extentions;

	errcode = 0;
	cl::Platform::get(&platforms);
	if (platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &this->clDevices) != CL_SUCCESS)
	{
		printf("Your platform does not have any GPU. Sorry.\n");
		throw std::exception();
	}
	this->clDevices[0].getInfo(CL_DEVICE_EXTENSIONS, &extentions);
	if (extentions.find("cl_APPLE_gl_sharing") == std::string::npos)
	{
		printf("Your GPU does not support sharing extention. Sorry.\n");
		throw std::exception();
	}
}

cl::Context		*GPUContext::GetOpenCLContext(void)
{
	cl::Context							*context;
	cl_int								errcode;
	CGLContextObj						kCGLContext = CGLGetCurrentContext();
	CGLShareGroupObj					sharegroup = CGLGetShareGroup(kCGLContext);

	cl_context_properties				properties[] {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
			(cl_context_properties)sharegroup, 0
			};
	context = new cl::Context(this->clDevices, properties, NULL, NULL, &errcode);
	if (errcode != CL_SUCCESS)
	{
		printf("Error : %d\n", errcode);
		throw std::exception();
	}
	return (context);
}

GLFWwindow		*GPUContext::GetGLFWWindow(int width, int height)
{
	GLFWwindow		*win;

	if (!glfwInit())
	{
		printf("Can't init GLFW\n");
		throw std::exception();
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	win = glfwCreateWindow(width, height, "ParSys", NULL, NULL);
	glfwMakeContextCurrent(win);
	return (win);
}

GPUContext::GPUContext(int width, int height) :
	width(width),
	height(height)
{
	this->GetCLDevices();
	this->glfwWindow = GetGLFWWindow(width, height);
	this->clContext = GetOpenCLContext();

// Ou tu veux que je foute ca ?
	glClearColor(0, 0, 0, 0);
//	glEnable(GL_DEPTH_TEST);
	glPointSize(1.0f);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc (GL_ONE, GL_SRC_ALPHA);
//	glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

GPUContext::~GPUContext()
{
	delete this->clContext;
	glfwDestroyWindow(this->glfwWindow);
	glfwTerminate();
}

cl::Context	const	&GPUContext::getCLContext()
{
	return (*this->clContext);
}

cl::Device const	&GPUContext::getCLDevice()
{
	return (this->clDevices[0]);
}

std::vector<cl::Device> const	&GPUContext::getCLDevices()
{
	return (this->clDevices);
}

GLFWwindow				*GPUContext::getGLFWContext()
{
	return (this->glfwWindow);
}

void				GPUContext::getCursorPos(double *x, double *y)
{
	glfwGetCursorPos(this->getGLFWContext(), x, y);
	*x = ((*x / this->width) - 0.5f) * 2.f;
	*y = (1.f - (*y / this->height) - 0.5f) * 2.f;
}
