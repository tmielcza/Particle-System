// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLVAO.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 18:57:45 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 21:02:41 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //


#include "GLVAO.hpp"

GLBuffer::GLBuffer(GLenum type, int type_size, int elem_size, int length) :
	length(length), type(type), elemLength(elem_size)
{
	glGenBuffers(1, &this->id);
	this->Bind();
	glBufferData(GL_ARRAY_BUFFER, length * type_size * elem_size, NULL, GL_STATIC_DRAW);
	this->Unbind();
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void	GLBuffer::Bind(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void	GLBuffer::Unbind(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int		GLBuffer::getId(void)
{
	return (this->id);
}

int		GLBuffer::getLength(void)
{
	return (this->length);
}

void	GLBuffer::BindAttrib(GLuint attribId)
{
	this->Bind();
	glVertexAttribPointer(attribId, this->elemLength, this->type, GL_FALSE, 0, NULL);
	this->Unbind();
}

template<>
GLBuffer	*GLBuffer::CreateGLBuffer<float>(int elems, int length)
{
	return (new GLBuffer(GL_FLOAT, sizeof(float), elems, length));
}


GLuint		GLVAO::GetVAO(void)
{
	GLuint		vao;

	glGenVertexArrays(1, &vao);
	return (vao);
}

GLVAO::GLVAO(int length, GLBuffer *buf) :
	id(GetVAO()),
	argsCount(1),
	length(length),
	buffs({buf}),
	ids(1)
{
	argsCount = 1;
}

GLVAO::~GLVAO()
{

}

void	GLVAO::Bind(void)
{
	glBindVertexArray(this->id);
}

void	GLVAO::Unbind(void)
{
	glBindVertexArray(0);
}

void	GLVAO::BindWithProgram(GLProgram &prog, std::string arg1)
{
	GLuint	loc;

	this->Bind();
	loc = prog.GetArgLocation(arg1.c_str());
	this->ids[0] = loc;
	this->buffs[0]->BindAttrib(loc);
	this->Unbind();
}

void	GLVAO::Draw(GLProgram &prog)
{
	this->Bind();
	prog.Bind();
	for (unsigned int i = 0; i < this->buffs.size(); i++)
	{
		glEnableVertexAttribArray(this->ids[i]);
	}
	glDrawArrays(GL_POINTS, 0, this->length);
	for (unsigned int i = 0; i < this->buffs.size(); i++)
	{
		glDisableVertexAttribArray(this->ids[i]);
	}
	prog.Unbind();
	this->Unbind();
}
