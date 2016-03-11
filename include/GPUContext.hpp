// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GPUContext.hpp                                     :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 22:54:14 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/11 23:42:55 by tmielcza         ###   ########.fr       //
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

inline void		ERR(std::string str)
{
	GLuint	err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("ERROR %s: %x;\n", str.c_str(),err);
	}
}

class GPUContext
{
private:
	std::vector<cl::Device>	clDevices;
	GLFWwindow				*glfwWindow;
	cl::Context				*clContext;
	int						width, height;

	cl::Context					*GetOpenCLContext(void);
	void						GetCLDevices(void);
	GLFWwindow					*GetGLFWWindow(int width, int height);

public:
					GPUContext(int width, int height);
					~GPUContext();

	cl::Context	const	&getCLContext();
	cl::Device const	&getCLDevice();
	std::vector<cl::Device> const	&getCLDevices();
	GLFWwindow			*getGLFWContext();
	void				getCursorPos(double *x, double *y);
	float				getX(void) {return this->width;}
	float				getY(void) {return this->height;}
};

#endif
