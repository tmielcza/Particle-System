// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   GLProgram.hpp                                      :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/05 01:32:00 by tmielcza          #+#    #+#             //
//   Updated: 2016/05/26 15:57:22 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef GLPROGRAM_HPP
# define GLPROGRAM_HPP

# include <cstdio>
# include <string>
# include <list>

# ifndef GLFW_INCLUDE_GLCOREARB
#	define GLFW_INCLUDE_GLCOREARB
# endif
# include "GLFW/glfw3.h"

# include "Matrix.hpp"

class GLProgram
{
private:
	std::list<GLuint>	shadersIds;
	GLuint				program;

	void	CheckShaderCompilation(GLuint shaderId);
	GLuint	CompileShader(std::string shader, GLuint shaderType);

public:
	GLProgram(std::string vertex, std::string fragment, std::string geometry = "");
	~GLProgram();

	void		Bind(void);
	void		Unbind(void);
	GLuint		getId(void);

	GLuint		GetArgLocation(std::string argName);

	// ?
	template<typename T>
	void		SetParam(std::string name, T *value, int nb)
		{
			//static_assert(false, "Can't set this type of parameter\n");
			(void)name;
			(void)value;
			(void)nb;
			fprintf(stderr, "Can't set this type of parameter\n");
			throw std::exception();
		}

	void		SetParam(std::string name, Matrix<4,4>);
};

template<> void		GLProgram::SetParam(std::string name, float *floats, int nb);

#endif
