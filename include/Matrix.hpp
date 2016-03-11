// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Matrix.hpp                                         :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: tmielcza <marvin@42.fr>                    +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2016/03/11 00:17:42 by tmielcza          #+#    #+#             //
//   Updated: 2016/03/11 01:52:41 by tmielcza         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <cmath>

template<size_t X, size_t Y>
class Matrix
{
public:
	float	data[Y][X];

	float		&Get(size_t x, size_t y)
	{
		return (this->data[y][x]);
	}

	template<size_t XX, size_t YY>
	Matrix<X, Y>	operator*(Matrix<XX, YY> &rhs)
	{
		Matrix		ret;

		static_assert (X == YY, "A width must match B height.");
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

	static Matrix<4,4>		Perspective(float fov, float aspect, float znear, float zfar)
	{
		Matrix<4,4>	ret = Identity();
		float		cota2;

		// Degrees to radians ?
		cota2 = 1.0f / tan((fov) / 2);
		ret.data[0][0] = (1.0f / aspect) * cota2;
		ret.data[1][1] = cota2;
		ret.data[2][2] = zfar / (zfar - znear);
		ret.data[2][3] = -1.0f;
		ret.data[3][2] = ((zfar * znear) / (zfar - znear));
		return (ret);
	}

	static Matrix<X,Y>		Identity(void)
	{
		Matrix<X,Y>		ret;

		for (size_t y = 0; y < Y; y++) {
			for (size_t x = 0; x < Y; x++) {
				if (x == y) {
					ret.data[y][x] = 1;
				} else {
					ret.data[y][x] = 0;
				}
			}
		}
		return (ret);
	}

	static Matrix<4,4>		Tanslation(float x, float y, float z)
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
};
