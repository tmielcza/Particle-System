// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/03 18:25:35 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/05 02:30:55 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "GPUContext.hpp"
#include "ParticleSystem.hpp"

int		main(void)
{
	std::string str =
		"kernel\n"
		"void	add(global float * const restrict a)\n"
		"{\n"
		"unsigned idx = get_global_id(0);\n"
		"//a[idx] = (float3)((float)idx, (float)idx, (float)idx);}\n"
		"a[idx * 3] = (float)idx;\n"
		"a[idx * 3 + 1] = (float)idx;\n"
		"a[idx * 3 + 2] = (float)idx;}\n"
			;
	printf("coucou\n");
	GPUContext context;

	try
	{
		ParticleSystem(context, 10, str);
	}
	catch (std::exception &e)
	{
		printf("Boum\n");
	}
	return (0);
}
