// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:11:08 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/06 01:46:25 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARTICLESYSYEM_HPP
# define PARTICLESYSYEM_HPP

# include "GPUContext.hpp"
# include "GLProgram.hpp"
# include "GLVAO.hpp"

struct Vector4
{
	float x, y, z, w;

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

class ParticleSystem
{
public:
			ParticleSystem(GPUContext &context, int size, std::string source);
			~ParticleSystem();

	void	ComputeParticles(void);
	void	RenderParticles(void);
	void	SetGravityCenter(Vector4 center);

private:
	GLBuffer			*glBuff;
	GLBuffer			*glBuffVelocities;
	cl::BufferGL		*clBuff;
	cl::BufferGL		*clBuffVelocities;
	cl::CommandQueue	queue;
	cl::Program			program;
	cl::Kernel			kernel;
	GLProgram			*glProgram;
	GPUContext			&context;
	int					size;
	GLVAO				*vao;
	Vector4				gravityCenter;

	void	Initialize(void);
};

#endif
