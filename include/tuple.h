/*
 * tuple.h
 *
 * Author: Weston Cook
 *
 * Distributed under the Mozilla Public Lincence 2.0
 *
 * Defines the struct Tuple:
 *   Represents an N-dimensional mathematical tuple
 * 
 * Also defines helper functions for performing vector arithmetic on Tuples, as
 * well as some other vector ops.
 */


#ifndef BRAZEN_TUPLE_H
#define BRAZEN_TUPLE_H

#include <cmath>  // std::sqrt, std::sin, std::cos
#include <array>  // std::array
#include <stdexcept>  // std::out_of_range
#include <string>  // std::to_string
#include <random> // std:: default_random_engine, std::uniform_real_distribution
#include <stdlib.h>
#include <iostream>

#define TUPLE_TYPE float
//#define WARN_ZERO_UNIT


/*
 * Struct: Tuple
 * -------------
 * Represents an N-Tuple and has several operations defined on it.
 *
 * Features:
 * -Scalar multiplication
 * -Vector addition/subtraction
 * -Return vector magnitude
 * -Return parallel unit vector
 * -Dot product
 * -Cross product (N-dimensional generalization)
 * -Return scalar projection of two vectors
 * -Return vector projection of two vectors
 *
 * NOTE: Explicit specializations are defined for _Size = 2, 3 for computational efficiency.
 * WARNING: For efficiency, Tuple uses type std::uint8_t for its dimension, so dimensions
 *	greater than 255 are not allowed.
 */
template <std::uint8_t _Size>
struct Tuple {
	std::array<TUPLE_TYPE, _Size> value;

	/*********************** CONSTRUCTORS/DESTRUCTORS ************************/

	/*
	 * Default constructor
	 * -------------------
	 * Initializes with all zeros if zeros is true.
	 */
	Tuple(bool zeros = true) {
		if (zeros)
			setZero();
	}

	/*
	 * Initializer list constructor
	 * ----------------------------
	 * Initializes with the values from the given initializer list.
	 */
	template <typename... T>
	Tuple(T... v) :
		value({ v... })
	{}

	/*
	 * std::array constructor
	 * ----------------------
	 * Initializes with the given std::array of values.
	 */
	template <typename T>
	Tuple(const std::array<T, _Size> v) :
		value(v)
	{}

	/*
	 * Copy constructor
	 * ----------------
	 * Initializes with the given Tuple.
	 */
	Tuple(const Tuple<_Size>& v) :
		value(v.value)
	{}
	
	/************************** OPERATOR OVERLOADS ***************************/

	/*
	 * Assignment operator (std::array)
	 * --------------------------------
	 * Assigns this Tuple to have the same value as the given std::array.
	 */
	template <typename T>
	const Tuple<_Size>& operator=(const std::array<T, _Size>& v) {
		for (std::uint8_t i = 0; i < _Size; i++)
			value[i] = v[i];
		
		return *this;
	}

	/*
	 * Assignment operator (Tuple)
	 * ---------------------------
	 * Assigns this Tuple to have the same value as the given Tuple.
	 */
	const Tuple<_Size>& operator=(const Tuple<_Size>& v) {
		for (std::uint8_t i = 0; i < _Size; i++)
			value[i] = v.value[i];
		
		return *this;
	}

	/*
	 * Index operator (mutable)
	 * ------------------------
	 * Return the component value at the given index.
	 */
	TUPLE_TYPE& operator[](std::uint8_t i) {
		if (i < _Size)
			return value[i];
		throw std::out_of_range("Index " + std::to_string(i) + " out of range for " + std::to_string(_Size) + "-Tuple.");
	}

	/*
	 * Index operator (immutable)
	 * --------------------------
	 * Return the component value at the given index.
	 */
	TUPLE_TYPE operator[](std::uint8_t i) const {
		if (i < _Size)
			return value[i];
		throw std::out_of_range("Index " + std::to_string(i) + " out of range for " + std::to_string(_Size) + "-Tuple.");
	}

	/************************ MANIPULATION OPERATIONS ************************/

	/*
	 * Function: setZero
	 * -----------------
	 * Sets all components to zero.
	 */
	void setZero(void) {
		for (std::uint8_t i = 0; i < _Size; i++)
			value[i] = 0.;
	}
};


/*
 * Explicit specialization of struct "Tuple" for 2-tuples.
 */
template <>
struct Tuple<2> {
	TUPLE_TYPE x;
	TUPLE_TYPE y;

	// Initialize with all zeros if zeros is true
	Tuple(bool zeros = true) {
		if (zeros)
			setZero();
	}

	// Initialize with the given values
	template <typename T>
	Tuple(T x, T y) :
		x(x), y(y)
	{}

	// Initialize with the given std::array of values
	template <typename T>
	Tuple(const std::array<T, 2> v) :
		x(v[0]), y(v[1])
	{}

	// Initialize with the given tuple
	Tuple(const Tuple<2>& v) :
		x(v.x), y(v.y)
	{}


	// Assign this tuple to have the same value as the argument std::array
	const Tuple<2>& operator=(const std::array<TUPLE_TYPE, 2>& v) {
		x = v[0];
		y = v[1];

		return *this;
	}

	// Assign this tuple to have the same value as the argument tuple
	const Tuple<2>& operator=(const Tuple<2>& v) {
		x = v.x;
		y = v.y;

		return *this;
	}


	// Index the components - mutable
	TUPLE_TYPE& operator[](std::uint8_t i) {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			throw std::out_of_range("Index " + std::to_string(i) + " out of range for 2-Tuple.");
		}
	}

	// Index the components - immutable
	TUPLE_TYPE operator[](std::uint8_t i) const {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			throw std::out_of_range("Index " + std::to_string(i) + " out of range for 2-Tuple.");
		}
	}


	// Set all elements to zero
	void setZero(void) {
		x = 0.;
		y = 0.;
	}
};


/*
 * Explicit specialization of struct "Tuple" for 3-tuples.
 */
template <>
struct Tuple<3> {
	TUPLE_TYPE x;
	TUPLE_TYPE y;
	TUPLE_TYPE z;

	// Initialize with all zeros if zeros is true
	Tuple(bool zeros = true) {
		if (zeros)
			setZero();
	}

	// Initialize with the given values
	template <typename T>
	Tuple(T x, T y, T z) :
		x(x), y(y), z(z)
	{}

	// Initialize with the given std::array of values
	template <typename T>
	Tuple(const std::array<T, 3> v) :
		x(v[0]), y(v[1]), z(v[2])
	{}

	// Initialize with the given tuple
	Tuple(const Tuple<3>& v) :
		x(v.x), y(v.y), z(v.z)
	{}


	// Assign this tuple to have the same value as the argument std::array
	const Tuple<3>& operator=(const std::array<TUPLE_TYPE, 3>& v) {
		x = v[0];
		y = v[1];
		z = v[2];

		return *this;
	}

	// Assign this tuple to have the same value as the argument tuple
	const Tuple<3>& operator=(const Tuple<3>& v) {
		x = v.x;
		y = v.y;
		z = v.z;

		return *this;
	}


	// Index the components - mutable
	TUPLE_TYPE& operator[](std::uint8_t i) {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Index " + std::to_string(i) + " out of range for 3-Tuple.");
		}
	}

	// Index the components - immutable
	TUPLE_TYPE operator[](std::uint8_t i) const {
		switch (i) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::out_of_range("Index " + std::to_string(i) + " out of range for 3-Tuple.");
		}
	}


	// Set all elements to zero
	void setZero(void) {
		x = 0.;
		y = 0.;
		z = 0.;
	}
};

/************************** TUPLE HELPER FUNCTIONS ***************************/

/*********** NOT IN PLACE ************/

/*
 * Multiplication operator (Tuple, scalar)
 * ---------------------------------------
 * Returns Tuple * scalar.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator*(const Tuple<_Size>& v, TUPLE_TYPE s) {
	Tuple<_Size> out(false);

	for (std::uint8_t i = 0; i < _Size; i++)
		out.value[i] = v.value[i] * s;

	return out;
}

inline Tuple<2> operator*(const Tuple<2>& v, TUPLE_TYPE s) {
	return Tuple<2>(v.x * s, v.y * s);
}

inline Tuple<3> operator*(const Tuple<3>& v, TUPLE_TYPE s) {
	return Tuple<3>(v.x * s, v.y * s, v.z * s);
}

/*
 * Multiplication operator (scalar, Tuple)
 * ---------------------------------------
 * Returns scalar * Tuple.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator*(TUPLE_TYPE s, const Tuple<_Size>& v) {
	Tuple<_Size> out(false);

	for (std::uint8_t i = 0; i < _Size; i++)
		out.value[i] = v.value[i] * s;

	return out;
}

inline Tuple<2> operator*(TUPLE_TYPE s, const Tuple<2>& v) {
	return Tuple<2>(v.x * s, v.y * s);
}

inline Tuple<3> operator*(TUPLE_TYPE s, const Tuple<3>& v) {
	return Tuple<3>(v.x * s, v.y * s, v.z * s);
}

/*
 * Division operator (Tuple, scalar)
 * ---------------------------------
 * Returns Tuple / scalar.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator/(const Tuple<_Size>& v, TUPLE_TYPE s) {
	Tuple<_Size> out(false);

	for (std::uint8_t i = 0; i < _Size; i++)
		out.value[i] = v.value[i] / s;

	return out;
}

inline Tuple<2> operator/(const Tuple<2>& v, TUPLE_TYPE s) {
	return Tuple<2>(v.x / s, v.y / s);
}

inline Tuple<3> operator/(const Tuple<3>& v, TUPLE_TYPE s) {
	return Tuple<3>(v.x / s, v.y / s, v.z / s);
}

/*
 * Addition operator (Tuple, Tuple)
 * --------------------------------
 * Returns Tuple1 + Tuple2.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator+(const Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	Tuple<_Size> out(false);

	for (std::uint8_t i = 0; i < _Size; i++)
		out.value[i] = v1.value[i] + v2.value[i];

	return out;
}

inline Tuple<2> operator+(const Tuple<2>& v1, const Tuple<2>& v2) {
	return Tuple<2>(v1.x + v2.x, v1.y + v2.y);
}

inline Tuple<3> operator+(const Tuple<3>& v1, const Tuple<3>& v2) {
	return Tuple<3>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

/*
 * Subtraction operator (Tuple, Tuple)
 * -----------------------------------
 * Returns Tuple1 - Tuple2.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator-(const Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	Tuple<_Size> out(false);

	for (std::uint8_t i = 0; i < _Size; i++)
		out.value[i] = v1.value[i] - v2.value[i];

	return out;
}

inline Tuple<2> operator-(const Tuple<2>& v1, const Tuple<2>& v2) {
	return Tuple<2>(v1.x - v2.x, v1.y - v2.y);
}

inline Tuple<3> operator-(const Tuple<3>& v1, const Tuple<3>& v2) {
	return Tuple<3>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

/*
 * Function: dot
 * -------------
 * Returns the dot product of Tuple1 and Tuple2.
 */
template <std::uint8_t _Size>
TUPLE_TYPE dot(const Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	TUPLE_TYPE sum = 0.;

	for (std::uint8_t i = 0; i < _Size; i++)
		sum += v1.value[i] * v2.value[i];

	return sum;
}

inline TUPLE_TYPE dot(const Tuple<2>& v1, const Tuple<2>& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline TUPLE_TYPE dot(const Tuple<3>& v1, const Tuple<3>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
 * Function: cross
 * ---------------
 * Returns the cross product of Tuple1 and Tuple2.
 */
inline Tuple<3> cross(const Tuple<3>& v1, const Tuple<3>& v2) {
	return Tuple<3>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

/*
 * Function: magnitudeSquared
 * --------------------------
 * Returns the squared magnitude of the given Tuple.
 */
template <std::uint8_t _Size>
TUPLE_TYPE magnitudeSquared(const Tuple<_Size>& v) {
	TUPLE_TYPE sum = 0.;

	for (std::uint8_t i = 0; i < _Size; i++)
		sum += v.value[i] * v.value[i];

	return sum;
}

inline TUPLE_TYPE magnitudeSquared(const Tuple<2>& v) {
	return v.x * v.x + v.y * v.y;
}

inline TUPLE_TYPE magnitudeSquared(const Tuple<3>& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

/*
 * Function: magnitude
 * -------------------
 * Returns the magnitude of the given Tuple.
 */
template <std::uint8_t _Size>
inline TUPLE_TYPE magnitude(const Tuple<_Size>& v) {
	return sqrt(magnitudeSquared(v));
}

inline TUPLE_TYPE magnitude(const Tuple<1>& v) {
	return std::abs(v.value[0]);
}

/*
 * Function: random_angle
 * ----------------------
 * Helper function for random_unit(...).
 * 
 * Contains static variables for generating random values.
 * 
 * Returns a distribution instance for generating uniformly distributed random
 * values.
 */
TUPLE_TYPE random_angle(void) {
	static std::random_device dev;
	static std::default_random_engine generator(dev());
	static std::uniform_real_distribution<TUPLE_TYPE> distribution(0, 6.28318530718);

	return distribution(generator);
};

/*
 * Function: random_unit
 * ---------------------
 * Return a randomly oriented unit vector that is uniformly distributed across
 * the surface of the unit N-sphere.
 */
template <std::uint8_t _Size>
Tuple<_Size> random_unit(void) {
	std::array<TUPLE_TYPE, _Size - 1> angles;  // Array containing the generalized spherical coordinates of the vector (without radius)
	Tuple<_Size> out(false);  // Output Tuple
	out.value[0] = 1.;  // Begin setting all output values to 1

						// Generate the spherical coordinate angles from a uniform distribution on [0, 2*pi)
	for (std::uint8_t i = 0; i < _Size - 1; i++) {
		angles[i] = random_angle();
		out.value[i + 1] = 1.;  // Finish setting all values of out to 1
	}

	// Convert the spherical coordinates to Euclidean coordinates
	//	(there has to be a more computationally efficient way
	//   to do this, but I haven't found it just yet)
	for (std::uint8_t i = 0; i < _Size - 1; i++) {
		out.value[i] *= std::cos(angles[i]);

		for (std::uint8_t j = i + 1; j < _Size; j++)
			out.value[j] *= std::sin(angles[i]);
	}

	return out;
}

template <>
Tuple<1> random_unit(void) {  // Return a randomly oriented unit vector from a distribution that is uniform across the surface of the unit 1-sphere.
	TUPLE_TYPE direction = random_angle() - 3.14159265358979323846;  // Uniformly distributed on [-pi, pi)

	return Tuple<1>(direction / std::abs(direction));
}

template <>
Tuple<2> random_unit(void) {  // Return a randomly oriented unit vector from a distribution that is uniform across the surface of the unit 2-sphere.
	TUPLE_TYPE theta = random_angle();

	return Tuple<2>(std::cos(theta), std::sin(theta));
}

template <>
Tuple<3> random_unit(void) {  // Return a randomly oriented unit vector from a distribution that is uniform across the surface of the unit 3-sphere.
	TUPLE_TYPE theta = random_angle();
	TUPLE_TYPE phi = random_angle();

	return Tuple<3>(std::sin(phi)*std::cos(theta), std::sin(phi)*std::sin(theta), std::cos(phi));
}

/*
 * Function: unit
 * --------------
 * Returns a unit Tuple parallel to the given Tuple.
 * 
 * If the given Tuple has magnitude zero and fake_it is true, return a randomly
 * oriented Tuple.
 * 
 * Args:
 *   fake_it: enables performing this operation on the zero-vector
 */
template <std::uint8_t _Size>
Tuple<_Size> unit(const Tuple<_Size>& v, bool fake_it = true) {
	// Compute the magnitude of the vector
	TUPLE_TYPE mag = magnitude(v);

	if (mag > 0.)  // Unit vector is defined
		return v / mag;
	// Division by zero, unit vector is not defined
#ifdef WARN_ZERO_UNIT
	std::cerr << "Warning: division by zero in function Tuple<" + std::to_string(_Size) + "> unit(const Tuple<" + std::to_string(_Size) + ">& v)." << std::endl;
#endif
	if (fake_it) {  // Return a randomly oriented unit vector
		return random_unit<_Size>();
	}
	else {
		std::cerr << "Finding the unit vector of the zero vector is fatal. Exiting." << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
 * Function: projection_scalar
 * ---------------------------
 * Returns the scalar projection of v1 onto v2.
 */
template <std::uint8_t _Size>
TUPLE_TYPE projection_scalar(const Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	return dot(v1, v2) / magnitude(v2);
}

/*
 * Function: projection_vector
 * ---------------------------
 * Returns the vector projection of v1 onto v2.
 */
template <std::uint8_t _Size>
Tuple<_Size> projection_vector(const Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	return v2 * (dot(v1, v2) / magnitudeSquared(v2));
}

/******** IN PLACE OPERATIONS ********/

/*
 * Assign-multiply operator (Tuple, scalar)
 * ----------------------------------------
 * Assigns the given Tuple to the value of Tuple * scalar.
 */
template <std::uint8_t _Size>
Tuple<_Size>& operator*=(Tuple<_Size>& v, TUPLE_TYPE s) {
	for (std::uint8_t i = 0; i < _Size; i++)
		v.value[i] *= s;

	return v;
}

Tuple<2>& operator*=(Tuple<2>& v, TUPLE_TYPE s) {
	v.x *= s;
	v.y *= s;

	return v;
}

Tuple<3>& operator*=(Tuple<3>& v, TUPLE_TYPE s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

/*
 * Assign-divide operator (Tuple, scalar)
 * ----------------------------------------
 * Assigns the given Tuple to the value of Tuple / scalar.
 */
template <std::uint8_t _Size>
Tuple<_Size>& operator/=(Tuple<_Size>& v, TUPLE_TYPE s) {
	for (std::uint8_t i = 0; i < _Size; i++)
		v.value[i] /= s;

	return v;
}

Tuple<2>& operator/=(Tuple<2>& v, TUPLE_TYPE s) {
	v.x /= s;
	v.y /= s;

	return v;
}

Tuple<3>& operator/=(Tuple<3>& v, TUPLE_TYPE s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;

	return v;
}

/*
 * Assign-add operator (Tuple, Tuple)
 * ----------------------------------------
 * Assigns the given Tuple to the value of Tuple1 + Tuple2.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator+=(Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	for (std::uint8_t i = 0; i < _Size; i++)
		v1.value[i] += v2.value[i];

	return v1;
}

Tuple<2> operator+=(Tuple<2>& v1, const Tuple<2>& v2) {
	v1.x += v2.x;
	v1.y += v2.y;

	return v1;
}

Tuple<3> operator+=(Tuple<3>& v1, const Tuple<3>& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}


/*
 * Assign-subtract operator (Tuple, Tuple)
 * ----------------------------------------
 * Assigns the given Tuple to the value of Tuple1 - Tuple2.
 */
template <std::uint8_t _Size>
Tuple<_Size> operator-=(Tuple<_Size>& v1, const Tuple<_Size>& v2) {
	for (std::uint8_t i = 0; i < _Size; i++)
		v1.value[i] -= v2.value[i];

	return v1;
}

Tuple<2> operator-=(Tuple<2>& v1, const Tuple<2>& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;

	return v1;
}

Tuple<3> operator-=(Tuple<3>& v1, const Tuple<3>& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

/***************************** OTHER OPERATIONS ******************************/

/*
 * Stream insertion operator (Tuple)
 * ---------------------------------
 * Prints the given Tuple to the given std::ostream.
 */
template <std::uint8_t _Size>
std::ostream& operator<<(std::ostream &s, const Tuple<_Size>& v) {
	s << "< " << v.value[0];

	for (std::uint8_t i = 1; i < _Size; i++)
		s << ", " << v.value[i];

	s << " >";

	return s;
}

std::ostream& operator<<(std::ostream &s, const Tuple<2>& v) {
	s << "< " << v.x << ", " << v.y << " >";

	return s;
}

std::ostream& operator<<(std::ostream &s, const Tuple<3>& v) {
	s << "< " << v.x << ", " << v.y << ", " << v.z << " >";

	return s;
}


#endif