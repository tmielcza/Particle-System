// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:11:08 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 00:43:36 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARTICLESYSYEM_HPP
# define PARTICLESYSYEM_HPP

# include "GPUContext.hpp"

template<typename T>
class GLBuffer
{
public:
			GLBuffer(int length)
		{
			glGenBuffers(1, &this->id);
			this->Bind();
			glBufferData(GL_ARRAY_BUFFER, length * sizeof(T), NULL, GL_STATIC_DRAW);
			this->Unbind();
		}

			~GLBuffer()
		{
			glDeleteBuffers(1, &this->id);
		}

	void	Bind(void)
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->id);
		}

	void	Unbind(void)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	int		getId(void)
		{
			return (this->id);
		}

private:
	GLuint	id;
};

struct Vector3
{
	float x, y, z;
};

class ParticleSystem
{
public:
			ParticleSystem(GPUContext &context, int size, std::string source);
			~ParticleSystem();

	void	ComputeParticles(void);
	void	RenderParticles(void);

private:
	GLBuffer<Vector3>	glBuff;
	cl::BufferGL		*clBuff;
	cl::CommandQueue	queue;
	cl::Program			program;
	cl::Kernel			kernel;
};

#endif
