// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Matrix.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/11 00:17:42 by tmielcza          #+#    #+#             //
//   Updated: 2016/05/27 19:14:51 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#pragma once

#include <cmath>

/*
template<size_t X, size_t Y>
class BaseMatrix
{
	float	data[Y][X];

	float	&operator()(size_t x, size_t y)
	{
		return (this->data[y][x]);
	}
};
*/

template<size_t X, size_t Y>
class Matrix
{
public:

	float	data[Y][X];

	float		&Get(size_t x, size_t y)
	{
		return (this->data[y][x]);
	}

	float		&operator()(size_t x, size_t y)
	{
		return (this->data[y][x]);
	}

	template<size_t XX>
	Matrix<X, Y>	operator*(Matrix<XX, X> &rhs)
	{
		Matrix<X, Y>	ret;

		for (size_t y = 0; y < Y; y++) {
			for (size_t x = 0; x < XX; x++) {
				ret.Get(x, y) = 0.0f;
				for (size_t i = 0; i < X; i++) {
					ret.Get(x, y) += this->Get(i, y) * rhs.Get(x, i);
				}
			}
		}
		return (ret);
	}

	Matrix<X, Y>	operator+(Matrix<X, Y> &rhs)
	{
		Matrix<X, Y>	ret;

		for (size_t y = 0; y < Y; y++) {
			for (size_t x = 0; x < X; x++) {
				ret(x, y) = (*this)(x, y) + rhs(x, y);
			}
		}
		return (ret);
	}

	static Matrix<4,4>		Perspective(float fov, float aspect, float znear, float zfar)
	{
		Matrix<4,4>	ret = Identity();

//*
		float		cota2;

		// Degrees to radians ?
		cota2 = 1.0f / tan((fov) / 2);
		ret.data[0][0] = cota2 / aspect;
		ret.data[1][1] = cota2;
//		ret.data[2][2] = zfar / (zfar - znear);
		ret.data[2][2] = (znear + zfar) / (znear - zfar);
		ret.data[3][2] = -1.0f;
		ret.data[2][3] = ((2 * zfar * znear) / (znear - zfar));
//		ret.data[3][2] = ((zfar * znear) / (zfar - znear));
//*/

/*
		float ymax = znear * tan(fov);
		float xmax = ymax * aspect;
		ret.data[0][0] = (2.0f * znear) / (2.0f * xmax);
		ret.data[1][1] = (2.0f * znear) / (2.0f * ymax);
		ret.data[2][2] = (-zfar - znear) / (zfar - znear);
		ret.data[2][3] = -1.0f;
		ret.data[3][2] = (-(2.0f * znear) * zfar) / (zfar - znear);
//*/

		return (ret);
	}

	static Matrix<X,Y>		Identity(void)
	{
		Matrix<X,Y>		ret;

		for (size_t y = 0; y < Y; y++) {
			for (size_t x = 0; x < X; x++) {
				if (x == y) {
					ret.data[y][x] = 1;
				} else {
					ret.data[y][x] = 0;
				}
			}
		}
		return (ret);
	}

	static Matrix<4,4>		Translation(float x, float y, float z)
	{
		Matrix<4,4>	ret;

		ret = Identity();
		ret.data[0][3] = x;
		ret.data[1][3] = y;
		ret.data[2][3] = z;
		return (ret);
	}

	static Matrix<4,4>		Scaling(float x, float y, float z)
	{
		Matrix<4,4>	ret;

		ret = Identity();
		ret.data[0][0] = x;
		ret.data[1][1] = y;
		ret.data[2][2] = z;
		return (ret);
	}

	static Matrix<4,4>		Rotation(float a, float x, float y, float z)
	{
		Matrix<4,4>	ret;
		float	c;
		float	s;

		// Degrees to radians ?
		c = cos(a);
		s = sin(a);
		ret = Identity();
		ret.data[0][0] = (x * x) + (1 - (x * x)) * c;
		ret.data[0][1] = x * y * (1 - c) - z * s;
		ret.data[0][2] = x * z * (1 - c) + y * s;
		ret.data[1][0] = x * y * (1 - c) + z * s;
		ret.data[1][1] = (y * y) + (1 - (y * y)) * c;
		ret.data[1][2] = y * z * (1 - c) - x * s;
		ret.data[2][0] = x * z * (1 - c) - y * s;
		ret.data[2][1] = y * z * (1 - c) + x * s;
		ret.data[2][2] = (z * z) + (1 - (z * z)) * c;
		return (ret);
	}

/*
	static Matrix<4,4>		Rotation(float angle, Vector<3> rot)
	{
		return (Rotation(angle, rot(0), rot(1), rot(2)));
	}
*/

	float const		*ToGLFloats(void)
	{
		return ((float const *)&this->data);
	}
};

template<size_t X>
class	Vector : public Matrix<1, X>
{
public:

	Vector() {}

	Vector<3>(float x, float y, float z) : Matrix<1, X>::Matrix {{{x}, {y}, {z}}}
	{
	}

	float		&Get(size_t x)
	{
		return (this->data[0][x]);
	}

	float		&operator()(size_t x)
	{
		return (this->data[0][x]);
	}

	Matrix<1, X>	&operator=(Matrix<1, X> const &rhs)
	{
		std::copy(&rhs.data[0][0], &rhs.data[0][0] + X, &this->data[0][0]);
		return (*this);
	}

	static Vector<4>	&Normalize(Vector<4>& vec)
	{
		auto homothecy = vec.data[3][0];

		for (auto i = 0; i < 3; i++) {
			vec.data[i][0] /= homothecy;
		}
		return vec;
	}

	float			Length(void)
	{
		float	sum = 0;
		for (auto i = 0u; i < X; i++) {
			auto val = this->Get(i);
			sum += val * val;
		}
		return std::sqrt(sum);
	}

	Vector<X>	operator-(Vector<X> &rhs)
	{
		Vector<X>	ret;

		for (auto x = 0u; x < X; x++) {
			ret(x) = (*this)(x) - rhs(x);
		}
		return (ret);
	}

};
