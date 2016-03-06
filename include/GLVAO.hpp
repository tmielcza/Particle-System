// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLVAO.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 18:48:25 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 21:02:37 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef GLVAO_HPP
# define GLVAO_HPP

# include <cstdio>
# include <vector>
# include "GLProgram.hpp"

# ifndef GLFW_INCLUDE_GLCOREARB
#	define GLFW_INCLUDE_GLCOREARB
# endif
# include "GLFW/glfw3.h"

class GLBuffer
{
public:
	GLBuffer(GLenum type, int type_size, int elem_size, int length);
	~GLBuffer();

	void	Bind(void);
	void	Unbind(void);
	int		getId(void);
	int		getLength(void);
	void	BindAttrib(GLuint attribId);

	template<typename T>
	static GLBuffer		*CreateGLBuffer(int elems, int length)
		{
			(void)elems;
			(void)length;
			printf("CreateGLBuffer doesnt support this type.\n");
			return (NULL);
		}

private:
	GLuint	id;
	int		length;
	GLenum	type;
	int		elemLength;
};

template<>
GLBuffer	*GLBuffer::CreateGLBuffer<float>(int elems, int length);


class GLVAO
{
public:
	GLVAO(int length, GLBuffer *buf);
	~GLVAO();

	void	Bind(void);
	void	Unbind(void);
	void	BindWithProgram(GLProgram &prog, std::string arg1);
	void	Draw(GLProgram &prog);

private:
	GLuint					id;
	int						argsCount;
	int						length;
	std::vector<GLBuffer *>	buffs;
	std::vector<GLuint>		ids;

	GLuint		GetVAO(void);
};

#endif
