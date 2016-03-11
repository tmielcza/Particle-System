// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Camera.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/11 17:01:23 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/11 19:54:28 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Camera.hpp"

Camera::Camera(void) :
	projection(Matrix<4,4>::Identity()),
	position (0, 0, 0)
{
}

Camera::Camera(Matrix<4,4> proj, Vector<3> pos = Vector<3>(0, 0, 0)) :
	projection(proj),
	position(pos)
{
}
