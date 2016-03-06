// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:11:08 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/06 00:52:53 by tmielcza         ###   ########.fr       //
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
};

class ParticleSystem
{
public:
			ParticleSystem(GPUContext &context, int size, std::string source);
			~ParticleSystem();

	void	ComputeParticles(void);
	void	RenderParticles(void);

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

	void	Initialize(void);
};

#endif
