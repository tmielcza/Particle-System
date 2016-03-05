// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLProgram.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 02:18:42 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 03:15:25 by tmielcza         ###   ########.fr       //
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

template<>
void		GLProgram::SetParam(std::string name, float value)
{
	GLint loc = glGetUniformLocation(this->program, name.c_str());
	glProgramUniform1f(this->program, loc, value);
}
