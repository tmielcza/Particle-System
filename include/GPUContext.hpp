// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GPUContext.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 22:54:14 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/03 23:21:05 by tmielcza         ###   ########.fr       //
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
	GLFWwindow		*glfwWindow;
	cl::Context		*clContext;

	cl::Context		*GetOpenCLContext(void);
	GLFWwindow		*GetGLFWWindow(void);

public:
					GPUContext();
					~GPUContext();
};

#endif
