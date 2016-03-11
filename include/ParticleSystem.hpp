// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   ParticleSystem.hpp                                 :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/04 15:11:08 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/11 17:12:23 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef PARTICLESYSYEM_HPP
# define PARTICLESYSYEM_HPP

# include <string>
# include <fstream>
# include <sstream>
# include "GPUContext.hpp"
# include "GLProgram.hpp"
# include "GLVAO.hpp"
# include "Camera.hpp"

struct Vector4
{
	float x, y, z, w;

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

class ParticleSystem
{
public:
					ParticleSystem() = delete;
					ParticleSystem(GPUContext &context, int size);
					~ParticleSystem();

	void			ComputeParticles(void);
	void			RenderParticles(void);
	void			SetGravityCenter(float x, float y, float z);
	std::string		ReadFile(std::string name);
	void			OnOff(void);
	void			SetGravity(bool position);
	void			ChangeInitForm(void);
	float			DeltaTime(void);

private:
	GLBuffer			*glBuff;
	GLBuffer			*glBuffVelocities;
	cl::BufferGL		*clBuff;
	cl::BufferGL		*clBuffVelocities;
	cl::CommandQueue	queue;
	cl::Program			program;
	cl::Kernel			kernel;
	cl::Kernel			kernelInitCube;
	GLProgram			*glProgram;
	GPUContext			&context;
	int					size;
	GLVAO				*vao;
	Vector4				gravityCenter;
	bool				run;
	bool				hasGravity;
	int					currentInitKernelId;
	Camera				camera;

	void	Initialize(cl::Kernel *ker);
};

#endif
