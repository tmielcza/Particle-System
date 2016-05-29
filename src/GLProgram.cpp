// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLProgram.cpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 02:18:42 by tmielcza          #+#    #+#             //
//   Updated: 2016/05/29 17:31:33 by tmielcza         ###   ########.fr       //
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

GLProgram::GLProgram(std::string vertex, std::string fragment, std::string geometry) :
//	vertShader(CompileShader(vertex, GL_VERTEX_SHADER)),
//	fragShader(CompileShader(fragment, GL_FRAGMENT_SHADER)),
//	fragShader(CompileShader(fragment, GL_FRAGMENT_SHADER)),
	program(glCreateProgram())
{
	GLuint vert = CompileShader(vertex, GL_VERTEX_SHADER);
	GLuint frag = CompileShader(fragment, GL_FRAGMENT_SHADER);
	GLuint geo;
	glAttachShader(this->program, vert);
	glAttachShader(this->program, frag);
	this->shadersIds.push_back(vert);
	this->shadersIds.push_back(frag);
	if (!geometry.empty()) {
		geo = CompileShader(geometry, GL_GEOMETRY_SHADER);
		glAttachShader(this->program, geo);
		this->shadersIds.push_back(geo);
	}
	glLinkProgram(this->program);
	// Maybe check if linking worked ? Nope ? Okay...
	glDetachShader(this->program, vert);
	glDetachShader(this->program, frag);
	if (!geometry.empty()) {
		glDetachShader(this->program, geo);
	}
}

GLProgram::~GLProgram()
{
	glDeleteProgram(this->program);
	for (auto shaderId : this->shadersIds) {
		glDeleteShader(shaderId);
	}
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
	case 1:
		glProgramUniform1fv(this->program, loc, 1, floats);
		break ;
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
