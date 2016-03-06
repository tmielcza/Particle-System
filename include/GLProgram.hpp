// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLProgram.hpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 01:32:00 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 20:58:42 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef GLPROGRAM_HPP
# define GLPROGRAM_HPP

# include <cstdio>
# include <string>

# ifndef GLFW_INCLUDE_GLCOREARB
#	define GLFW_INCLUDE_GLCOREARB
# endif
# include "GLFW/glfw3.h"

class GLProgram
{
private:
	GLuint		vertShader;
	GLuint		fragShader;
	GLuint		program;

	void	CheckShaderCompilation(GLuint shaderId);
	GLuint	CompileShader(std::string shader, GLuint shaderType);

public:
	GLProgram(std::string vertex, std::string fragment);
	~GLProgram();

	void		Bind(void);
	void		Unbind(void);
	GLuint		getId(void);

	GLuint		GetArgLocation(std::string argName);

	template<typename T>
	void		SetParam(std::string name, T value)
		{
			(void)name;
			(void)value;
			fprintf(stderr, "Can't set this type of parameter\n");
			throw std::exception();
		}
};

template<> void		GLProgram::SetParam(std::string name, float value);

#endif
