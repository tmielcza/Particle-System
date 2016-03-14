// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 18:25:35 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/14 16:38:37 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <cstdlib>
#include "GPUContext.hpp"
#include "ParticleSystem.hpp"

class	Key
{
public:
	Key(GPUContext &context, int key) : 
		context(context), key(key), isPressed(false)
	{
	}

	bool	Pressed(void)
	{
		if (glfwGetKey(this->context.getGLFWContext(), this->key) == GLFW_PRESS) {
			if (!this->isPressed) {
				this->isPressed = true;
				return (true);
			}
		} else {
			this->isPressed = false;
		}
		return (false);
	}

private:
	GPUContext	&context;
	int					key;
	bool				isPressed;
};

int		main(int ac, char **av)
{
	GPUContext	*context;
	double		x, y;
	bool		isLaunched = false;
	bool		hasGravity = true;
	int			size;

	if (ac != 2)
	{
		printf("Usage: %s <particles_number>\n", av[0]);
		return (1);
	}
	context = new GPUContext(1920, 1080);
	Key		keyPause(*context, GLFW_KEY_SPACE);
	Key		keyChangeForm(*context, GLFW_KEY_F);
	// Overflow ??
	Key		keySetGravity(*context, GLFW_KEY_G);
	float	delta;
	size = atoi(av[1]);
	try {
		auto ps = ParticleSystem(*context, size);
		while (glfwGetKey(context->getGLFWContext(), GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
			glfwPollEvents();
			delta = ps.DeltaTime();
			glfwSetWindowTitle(context->getGLFWContext(), std::to_string(1.f / delta).c_str());
			if (keyPause.Pressed()) {
				isLaunched = true;
				ps.OnOff();
			}

			if (keyChangeForm.Pressed() && !isLaunched) {
				ps.ChangeInitForm();
			}

			printf("lO\n");
			(void)hasGravity;

			if (keySetGravity.Pressed()) {
				hasGravity = !hasGravity;
				ps.SetGravity(hasGravity);
			}


			//Beurk
			if (glfwGetKey(context->getGLFWContext(), GLFW_KEY_W) == GLFW_PRESS) {
				ps.Move(Vector<3>(0.0f, 0.0f, 0.1f));
			}
			printf("ZU\n");
			context->getGLFWContext();
			printf("%p \n", context);

			ps.ComputeParticles();
			ps.RenderParticles();
			printf("lu\n");
			if (glfwGetMouseButton(context->getGLFWContext(), 0) == GLFW_PRESS) {
				context->getCursorPos(&x, &y);
				ps.SetGravityCenter(x, y, 0.f);
			}
			printf("la\n");
		}
	}
	catch (std::exception &e)
	{
		printf("ERROR: %s\n", e.what());
	}
	return (0);
}
