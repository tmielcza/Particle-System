// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 18:25:35 by tmielcza          #+#    #+#             //
//   Updated: 2016/05/29 15:58:56 by tmielcza         ###   ########.fr       //
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
	Key		keySetGravity(*context, GLFW_KEY_G);
	float	delta;
	size = atoi(av[1]);

	// USE "USERPOINTER" WITH GLFW FOR CALLBACKS !!!!!!

	try {
		auto ps = ParticleSystem(*context, size);
		while (glfwGetKey(context->getGLFWContext(), GLFW_KEY_ESCAPE) == GLFW_RELEASE) {

			glfwPollEvents();
			delta = ps.DeltaTime();
			glfwSetWindowTitle(context->getGLFWContext(), std::to_string(1.f / delta).c_str());
			if (keyPause.Pressed()) {
				isLaunched = true;
				ps.OnOff();
			} if (keyChangeForm.Pressed() && !isLaunched) {
				ps.ChangeInitForm();
			} if (keySetGravity.Pressed()) {
				hasGravity = !hasGravity;
				ps.SetGravity(hasGravity);

			} if (glfwGetKey(context->getGLFWContext(), GLFW_KEY_W) == GLFW_PRESS) {
				ps.Move(Vector<3>(0.0f, 0.0f, 0.01f));
			} if (glfwGetKey(context->getGLFWContext(), GLFW_KEY_S) == GLFW_PRESS) {
				ps.Move(Vector<3>(0.0f, 0.0f, -0.01f));
			} if (glfwGetKey(context->getGLFWContext(), GLFW_KEY_A) == GLFW_PRESS) {
				ps.Move(Vector<3>(-0.01f, 0.0f, 0.0f));
			} if (glfwGetKey(context->getGLFWContext(), GLFW_KEY_D) == GLFW_PRESS) {
				ps.Move(Vector<3>(0.01f, 0.0f, 0.0f));
			}

			if (glfwGetMouseButton(context->getGLFWContext(), 1) == GLFW_PRESS) {
				context->getCursorPos(&x, &y);
				ps.CameraPitch(y * 4.f);
				ps.CameraYaw(x * 4.f);
			} else {ps.CameraPitch(0); ps.CameraYaw(0);}

			context->getGLFWContext();
			ps.ComputeParticles();
			ps.RenderParticles();
			if (glfwGetMouseButton(context->getGLFWContext(), 0) == GLFW_PRESS) {
				context->getCursorPos(&x, &y);
				ps.SetGravityCenter(x * 5.0f, y * 5.0f, 0.f);
			}
		}
	}
	catch (std::exception &e)
	{
		printf("ERROR: %s\n", e.what());
	}
	return (0);
}
