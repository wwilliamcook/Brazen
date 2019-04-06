//vector.h
//Written by Weston Cook
//18 Aug. 2018
//defines the structs Vector2D and Vector3D, along with related helper functions

#ifndef PARTICLE_SIMULATOR_VECTOR_H
#define PARTICLE_SIMULATOR_VECTOR_H

#include <cmath>
#include <iostream>

namespace PhysicsSimulator {
	template <class dtype>
	struct Vector2D {
		dtype x, y;

		Vector2D(void) :
			x(0), y(0)
		{}
		Vector2D(dtype x, dtype y) :
			x(x), y(y)
		{}
		Vector2D(const Vector2D<dtype>& v) :
			x(v.x), y(v.y)
		{}

		Vector2D& operator=(const Vector2D<dtype>& v)
		{
			x = v.x;
			y = v.y;

			return *this;
		}

		Vector2D& setZero(void) {
			x = 0;
			y = 0;

			return *this;
		}
	};

	template <class dtype>
	struct Vector3D {
		dtype x, y, z;

		Vector3D(void) :
			x(0), y(0), z(0)
		{}
		Vector3D(dtype x, dtype y, dtype z) :
			x(x), y(y), z(z)
		{}
		Vector3D(const Vector3D<dtype>& v) :
			x(v.x), y(v.y), z(v.z)
		{}
		explicit Vector3D(const Vector2D<dtype>& v) :
			x(v.x), y(v.y), z(0)
		{}

		Vector3D& operator=(const Vector3D<dtype>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;

			return *this;
		}
		Vector3D& operator=(const Vector2D<dtype>& v)
		{
			x = v.x;
			y = v.y;
			z = 0;

			return *this;
		}

		Vector3D& setZero(void) {
			x = 0;
			y = 0;
			z = 0;

			return *this;
		}
	};

	/*************VECTOR HELPER FUNCTIONS***********/
	/*************NOT IN PLACE************/
	//SCALAR VECTOR MULTIPLICATION
	//vector, scalar
	template <class dtype, class dtype2>
	inline Vector2D<dtype> operator*(const Vector2D<dtype>& v, dtype2 s) {
		return Vector2D<dtype>(v.x * s, v.y * s);
	}
	template <class dtype, class dtype2>
	inline Vector3D<dtype> operator*(const Vector3D<dtype>& v, dtype2 s) {
		return Vector3D<dtype>(v.x * s, v.y * s, v.z * s);
	}
	//scalar, vector
	template <class dtype, class dtype2>
	inline Vector2D<dtype> operator*(dtype2 s, const Vector2D<dtype>& v) {
		return Vector2D<dtype>(v.x * s, v.y * s);
	}
	template <class dtype, class dtype2>
	inline Vector3D<dtype> operator*(dtype2 s, const Vector3D<dtype>& v) {
		return Vector3D<dtype>(v.x * s, v.y * s, v.z * s);
	}
	//SCALAR VECTOR DIVISION
	template <class dtype, class dtype2>
	inline Vector2D<dtype> operator/(const Vector2D<dtype>& v, dtype2 s) {
		return Vector2D<dtype>(v.x / s, v.y / s);
	}
	template <class dtype, class dtype2>
	inline Vector3D<dtype> operator/(const Vector3D<dtype>& v, dtype2 s) {
		return Vector3D<dtype>(v.x / s, v.y / s, v.z / s);
	}
	//VECTOR ADDITION
	template <class dtype>
	inline Vector2D<dtype> operator+(const Vector2D<dtype>& v1, const Vector2D<dtype>& v2) {
		return Vector2D<dtype>(v1.x + v2.x, v1.y + v2.y);
	}
	template <class dtype>
	inline Vector3D<dtype> operator+(const Vector3D<dtype>& v1, const Vector3D<dtype>& v2) {
		return Vector3D<dtype>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	//VECTOR SUBTRACTION
	template <class dtype>
	inline Vector2D<dtype> operator-(const Vector2D<dtype>& v1, const Vector2D<dtype>& v2) {
		return Vector2D<dtype>(v1.x - v2.x, v1.y - v2.y);
	}
	template <class dtype>
	inline Vector3D<dtype> operator-(const Vector3D<dtype>& v1, const Vector3D<dtype>& v2) {
		return Vector3D<dtype>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}
	//DOT PRODUCT
	template <class dtype>
	inline dtype dot(const Vector2D<dtype>& v1, const Vector2D<dtype>& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	template <class dtype>
	inline dtype dot(const Vector3D<dtype>& v1, const Vector3D<dtype>& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	//CROSS PRODUCT
	template <class dtype>
	inline Vector3D<dtype> cross(const Vector3D<dtype>& v1, const Vector3D<dtype>& v2) {
		return Vector3D<dtype>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}
	//MAGNITUDE
	template <class dtype>
	inline dtype magnitudeSquared(const Vector2D<dtype>& v) {
		return dot(v, v);
	}
	template <class dtype>
	inline dtype magnitudeSquared(const Vector3D<dtype>& v) {
		return dot(v, v);
	}
	template <class dtype>
	inline dtype magnitude(const Vector2D<dtype>& v) {
		return sqrtl(dot(v, v));
	}
	template <class dtype>
	inline dtype magnitude(const Vector3D<dtype>& v) {
		return sqrtl(dot(v, v));
	}
	//UNIT
	template <class dtype>
	Vector2D<dtype> unit(const Vector2D<dtype>& v) {
		dtype mag = magnitude(v);
		if (mag > 0)
			return v / mag;
		else {
			std::cerr << "Error: division by zero in vector unit()\nAborting." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	template <class dtype>
	Vector3D<dtype> unit(const Vector3D<dtype>& v) {
		dtype mag = magnitude(v);
		if (mag > 0)
			return v / mag;
		else {
			std::cerr << "Error: division by zero in vector unit()\nAborting." << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	/***************IN PLACE**************/
	//SCALAR VECTOR MULTIPLICATION
	template <class dtype, class dtype2>
	Vector2D<dtype>& operator*=(Vector2D<dtype>& v, dtype2 s) {
		v.x *= s;
		v.y *= s;

		return v;
	}
	template <class dtype, class dtype2>
	Vector3D<dtype>& operator*=(Vector3D<dtype>& v, dtype2 s) {
		v.x *= s;
		v.y *= s;
		v.z *= s;

		return v;
	}
	//SCALAR VECTOR DIVISION
	template <class dtype, class dtype2>
	Vector2D<dtype>& operator/=(Vector2D<dtype>& v, dtype2 s) {
		v.x /= s;
		v.y /= s;

		return v;
	}
	template <class dtype, class dtype2>
	Vector3D<dtype>& operator/=(Vector3D<dtype>& v, dtype2 s) {
		v.x /= s;
		v.y /= s;
		v.z /= s;

		return v;
	}
	//VECTOR ADDITION
	template <class dtype>
	Vector2D<dtype> operator+=(Vector2D<dtype>& v1, const Vector2D<dtype>& v2) {
		v1.x += v2.x;
		v1.y += v2.y;

		return v1;
	}
	template <class dtype>
	Vector3D<dtype>& operator+=(Vector3D<dtype>& v1, const Vector3D<dtype>& v2) {
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;

		return v1;
	}
	//VECTOR SUBTRACTION
	template <class dtype>
	Vector2D<dtype>& operator-=(Vector2D<dtype>& v1, const Vector2D<dtype>& v2) {
		v1.x -= v2.x;
		v1.y -= v2.y;

		return v1;
	}
	template <class dtype>
	Vector3D<dtype>& operator-=(Vector3D<dtype>& v1, const Vector3D<dtype>& v2) {
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;

		return v1;
	}
	//UNIT
	template <class dtype>
	Vector2D<dtype>& unitInplace(Vector2D<dtype>& v) {
		dtype mag = magnitude(v);
		v.x /= mag;
		v.y /= mag;
		v.z /= mag;

		return v;
	}

	//STREAM INSERTION
	template <class dtype>
	std::ostream& operator<<(std::ostream &s, const Vector2D<dtype>& v) {
		s << "< " << v.x << ", " << v.y << " >";

		return s;
	}
	template <class dtype>
	std::ostream& operator<<(std::ostream &s, const Vector3D<dtype>& v) {
		s << "< " << v.x << ", " << v.y << ", " << v.z << " >";

		return s;
	}
}

#endif