// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GPUContext.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 22:54:14 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/04 19:36:03 by tmielcza         ###   ########.fr       //
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

class GPUContext
{
private:
	std::vector<cl::Device>	clDevices;
	GLFWwindow				*glfwWindow;
	cl::Context				*clContext;

	cl::Context					*GetOpenCLContext(void);
	void						GetCLDevices(void);
	GLFWwindow					*GetGLFWWindow(void);

public:
					GPUContext();
					~GPUContext();

	cl::Context	const	&getCLContext();
	cl::Device const	&getCLDevice();
	std::vector<cl::Device> const	&getCLDevices();
	GLFWwindow	const	&getGLFWContext();
};

#endif
