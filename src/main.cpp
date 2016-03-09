// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 18:25:35 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/09 02:41:33 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <cstdlib>
#include "GPUContext.hpp"
#include "ParticleSystem.hpp"

int		main(int ac, char **av)
{
	GPUContext	*context;
	double		x, y;

	if (ac != 2)
	{
		printf("Usage: %s <particles_number>\n", av[0]);
		return (1);
	}
	context = new GPUContext(1920, 1080);
	try {
		auto ps = ParticleSystem(*context, atoi(av[1]));
		while (glfwGetKey(context->getGLFWContext(), GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
			glfwPollEvents();
			ps.ComputeParticles();
			ps.RenderParticles();
			if (glfwGetMouseButton(context->getGLFWContext(), 0) == GLFW_PRESS) {
				context->getCursorPos(&x, &y);
				ps.SetGravityCenter(x, y, 0.f);
			}
		}
	}
	catch (std::exception &e)
	{
		printf("ERROR: %s\n", e.what());
	}
	return (0);
}
