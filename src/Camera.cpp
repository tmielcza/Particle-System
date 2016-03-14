// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Camera.cpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/11 17:01:23 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/14 16:20:07 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <algorithm>
#include "Camera.hpp"

Camera::Camera(void) :
	projection(Matrix<4,4>::Identity()),
	position (0, 0, 0)
{
}

Camera::Camera(Matrix<4,4> proj, Vector<3> pos) :
	projection(proj),
	position(pos)
{
}

Camera::~Camera()
{
}

void	Camera::Pitch(float a)
{
	this->rotation(0) += a;
	this->rotation(0) = std::max(std::min(this->rotation(0), 90.0f), -90.0f);
}

void	Camera::Yaw(float a)
{
	this->rotation(1) += a;
}

void	Camera::Move(Vector<3> vector)
{
	this->position = this->position + vector;
}

Matrix<4,4>		Camera::GetMatrix(void)
{
	Matrix<4,4>	ret;

	ret = Matrix<4,4>::Identity();
	ret = Matrix<4,4>::Rotation(this->rotation(0), 1, 0, 0) * ret;
	ret = Matrix<4,4>::Rotation(this->rotation(1), 0, 1, 0) * ret;
	ret = Matrix<4,4>::Translation(this->position(0), this->position(1), this->position(2)) * ret;

	ret = this->projection * ret;

/*
	printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		   ret(0,0),ret(0,1),ret(0,2),ret(0,3),
		   ret(1,0),ret(1,1),ret(1,2),ret(1,3),
		   ret(2,0),ret(2,1),ret(2,2),ret(2,3),
		   ret(3,0),ret(3,1),ret(3,2),ret(3,3)
		);
*/

	return (ret);
/*
	MULT(cam_mat, mat_rotation(cam_rot.vec.x, 1.0f, 0.0f, 0.0f), cam_mat);
    MULT(cam_mat, mat_rotation(cam_rot.vec.y, 0.0f, 1.0f, 0.0f), cam_mat);
    MULT(cam_mat, mat_translation(cam_pos.x, cam_pos.y, cam_pos.z), cam_mat);
    MULT(cam_mat, mat_scaling(1, 1, -1), cam_mat);
*/
}
