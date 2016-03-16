// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLProgram.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 02:18:42 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/16 00:23:40 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "GLProgram.hpp"

void	GLProgram::CheckShaderCompilation(GLuint shaderId)
{
	GLint	isCompiled;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint			maxLen;
		std::string		errStr;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLen);
		errStr.resize(maxLen);
		glGetShaderInfoLog(shaderId, maxLen, &maxLen, &errStr[0]);
		printf("Can't compile shader:\n%s", errStr.c_str());
		glDeleteShader(shaderId);
		throw std::exception();
	}
}

GLuint	GLProgram::CompileShader(std::string shader, GLuint shaderType)
{
	GLuint		shaderId = glCreateShader(shaderType);
	const char	*source = shader.c_str();

	glShaderSource(shaderId, 1, &source, 0);
	glCompileShader(shaderId);
#ifdef DEBUG
	CheckShaderCompilation(shaderId);
#endif
	return (shaderId);
}

GLProgram::GLProgram(std::string vertex, std::string fragment) :
	vertShader(CompileShader(vertex, GL_VERTEX_SHADER)),
	fragShader(CompileShader(fragment, GL_FRAGMENT_SHADER)),
	program(glCreateProgram())
{
	glAttachShader(this->program, this->vertShader);
	glAttachShader(this->program, this->fragShader);
	glLinkProgram(this->program);
	// Maybe check if linking worked ? Nope ? Okay...
	glDetachShader(this->program, this->vertShader);
	glDetachShader(this->program, this->fragShader);
}

GLProgram::~GLProgram()
{
	glDeleteProgram(this->program);
	glDeleteShader(this->vertShader);
	glDeleteShader(this->fragShader);
}

void		GLProgram::Bind(void)
{
	glUseProgram(this->program);
}

void		GLProgram::Unbind(void)
{
	glUseProgram(0);
}

GLuint		GLProgram::getId(void)
{
	return (this->program);
}

GLuint		GLProgram::GetArgLocation(std::string argName)
{
	return (glGetAttribLocation(this->getId(), argName.c_str()));
}

template<>
void		GLProgram::SetParam(std::string name, float *floats, int nb)
{
	GLint loc = glGetUniformLocation(this->program, name.c_str());
	switch (nb)
	{
	case 4:
		glProgramUniform4fv(this->program, loc, 1, floats);
		break ;
	default:
		break ;
	}
}

void		GLProgram::SetParam(std::string name, Matrix<4,4> mat)
{
	GLint loc = glGetUniformLocation(this->program, name.c_str());
	glProgramUniformMatrix4fv(this->program, loc, 1, GL_TRUE, mat.ToGLFloats());
}
