// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Camera.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/11 15:43:21 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/11 17:21:01 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#pragma once

#include "Matrix.hpp"

class Camera
{
private:
	Matrix<4,4>		projection;
	Vector<3>		position;

public:
	Camera(void);
	Camera(Matrix<4,4> proj, Vector<3> pos);// = {{0, 0, 0}});

};
