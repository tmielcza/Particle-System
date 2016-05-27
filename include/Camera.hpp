// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Camera.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/11 15:43:21 by tmielcza          #+#    #+#             //
//   Updated: 2016/05/27 14:33:52 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#pragma once

#include "Matrix.hpp"

class Camera
{
private:
	Matrix<4,4>		projection;
	Vector<3>		position;
	Vector<3>		rotation;

public:
	Camera(void);
//	Camera(Matrix<4,4> proj, Vector<3> pos);
	Camera(Matrix<4,4> proj, Vector<3> pos = Vector<3>(0, 0, 0));
	~Camera(void);

	void		Pitch(float a);
	void		Yaw(float a);
	void		Move(Vector<3> vector);
	Matrix<4,4>	GetMatrix(void);
	Vector<3>&	GetPosition(void);
};
