// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   OpenCLWrapper.hpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 17:13:58 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/03 20:54:07 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef OPENCLWRAPPER_HPP
# define OPENCLWRAPPER_HPP

# include <cstdio>
# include <OpenGL/OpenGL.h>
# include <OpenCL/cl_gl_ext.h>
# include "OpenCL/cl.hpp"

# define GLFW_INCLUDE_GLCOREARB
# include "GLFW/glfw3.h"

namespace OpenCLWrapper
{
	int		GetOpenCLContext(void)
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
			return (-1);
		}
		devices[0].getInfo(CL_DEVICE_EXTENSIONS, &extentions);
		if (extentions.find("cl_APPLE_gl_sharing") == std::string::npos)
		{
			printf("Your GPU does not support sharing extention. Sorry.\n");
			return (-1);
		}
		cl_context_properties				properties[] {
			CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
			(cl_context_properties)sharegroup, 0
				};
		context = new cl::Context(devices, properties, NULL, NULL, &errcode);
		if (errcode != CL_SUCCESS)
		{
			printf("Error : %d\n", errcode);
			return (-1);
		}
		return (0);
	}

	GLFWwindow	*InitGLFW(void)
	{
		GLFWwindow		*win;

		if (!glfwInit())
		{
			return (NULL);
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

	int		Init(void)
	{
		GLFWwindow		*win;

		win = InitGLFW();
		GetOpenCLContext();
		glfwDestroyWindow(win);
		glfwTerminate();
		return (0);
	}
}

#endif
