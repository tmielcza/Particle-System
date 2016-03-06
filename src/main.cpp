// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 18:25:35 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/06 01:23:14 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "GPUContext.hpp"
#include "ParticleSystem.hpp"

int		main(void)
{
	std::string str =
		"kernel\n"
		"void	add(const int num, global float4 * const restrict a)\n"
		"{\n"
		"unsigned int idx = get_global_id(0);\n"
		"unsigned int k = ceil(cbrt((float)num));\n"
		"float x = (float)(idx % k) / (float)k * 0.5f;\n"
		"float y = (float)((idx / k) % k) / (float)k * 0.5f;\n"
		"float z = (float)(idx / (k * k)) / (float)k * 0.5f;\n"
		"float4 pos = (float4)(x, y, z, 1.0);\n"
		"a[idx] = pos;}\n"
		"\n"
		"kernel\n"
		"void	update(const int num, global float4 * const restrict a,\n"
		"global float4 * const restrict velocities)\n"
		"{\n"
		"unsigned int idx = get_global_id(0);\n"
		"float4 gravity = (float4)(0, 0, 0, 1) - a[idx];\n"
		"gravity = normalize(gravity);\n"
		"gravity = gravity * 0.001f;\n"
		"velocities[idx] += gravity;\n"
		"a[idx] += velocities[idx];}"
		;
	GPUContext context;

	try
	{
		auto ps = ParticleSystem(context, 100, str);
//		ps.ComputeParticles();
		ps.RenderParticles();
		while (1) {
			glfwPollEvents();
			ps.ComputeParticles();
			ps.RenderParticles();
/*
			if (glfwGetMouseButton(context.getGLFWWindow(), 0) == GLFW_PRESS)
			{
				double	x, y;

				glfwGetCursorPos(context.getGLFWWindow(), &x, &y);
				ps.SetGravityCenter({x + 0.5f, y + 0.5f, 0.0f, 1.0f});
			}
*/
		}
	}
	catch (std::exception &e)
	{
		printf("Boum\n");
	}
	return (0);
}
