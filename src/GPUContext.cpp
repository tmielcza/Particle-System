// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GPUContext.cpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 22:33:08 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/03 23:21:55 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "GPUContext.hpp"

cl::Context		*GPUContext::GetOpenCLContext(void)
{
	std::vector<cl::Platform>			platforms;
	std::vector<cl::Device>				devices;
	std::string							extentions;
	cl::Context							*context;
	cl_int								errcode;
	CGLContextObj						kCGLContext = CGLGetCurrentContext();
	CGLShareGroupObj					sharegroup = CGLGetShareGroup(kCGLContext);

	errcode = 0;
	cl::Platform::get(&platforms);
	if (platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices) != CL_SUCCESS)
	{
		printf("Your platform does not have any GPU. Sorry.\n");
		throw std::exception();
	}
	devices[0].getInfo(CL_DEVICE_EXTENSIONS, &extentions);
	if (extentions.find("cl_APPLE_gl_sharing") == std::string::npos)
	{
		printf("Your GPU does not support sharing extention. Sorry.\n");
		throw std::exception();
	}
	cl_context_properties				properties[] {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
			(cl_context_properties)sharegroup, 0
			};
	context = new cl::Context(devices, properties, NULL, NULL, &errcode);
	if (errcode != CL_SUCCESS)
	{
		printf("Error : %d\n", errcode);
		throw std::exception();
	}
	return (context);
}

GLFWwindow		*GPUContext::GetGLFWWindow(void)
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
	win = glfwCreateWindow(640, 480, "ParSys", NULL, NULL);
	glfwMakeContextCurrent(win);
//		glfwSetCursorPosCallback(win, update_cursor_position);
//		glfwSetMouseButtonCallback(win, update_mouse_click);
//		glfwSetScrollCallback(win, update_mouse_wheel);
//		get_input_manager_instance()->win = win;
//		glEnable(GL_DEPTH_TEST);
//		glDepthFunc(GL_LESS);
	return (win);
}

GPUContext::GPUContext() : glfwWindow(GetGLFWWindow()), clContext(GetOpenCLContext())
{
}

GPUContext::~GPUContext()
{
	delete this->clContext;
	glfwDestroyWindow(this->glfwWindow);
	glfwTerminate();
}
