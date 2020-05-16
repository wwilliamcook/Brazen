/*
 * vector_ops.c
 * 
 * Implementation file for vector_ops.h
 */

#include "vector_ops.h"

#include <math.h>

/************************* FUNCTION IMPLEMENTATIONS **************************/

/*
 * function bn2VectZeros
 * 
 * Returns a new zero vector.
 */
bn2Vect bn2VectZeros(void) {
    bn2Vect out;
    
    out.x = 0.;
    out.y = 0.;

    return out;
}

/*
 * function bn3VectZeros
 * 
 * Returns a new zero vector.
 */
bn3Vect bn3VectZeros(void) {
    bn3Vect out;
    
    out.x = 0.;
    out.y = 0.;
    out.z = 0.;

    return out;
}

/*
 * function bn2VectSetZero
 * 
 * Sets all components of the given vector to zero.
 */
void bn2VectSetZero(bn2Vect *v) {
    (*v).x = 0.;
    (*v).y = 0.;
}

/*
 * function bn3VectSetZero
 * 
 * Sets all components of the given vector to zero.
 */
void bn3VectSetZero(bn3Vect *v) {
    (*v).x = 0.;
    (*v).y = 0.;
    (*v).z = 0.;
}

/*
 * function bn2VectSum
 * 
 * Returns v1 + v2.
 */
bn2Vect bn2VectSum(bn2Vect v1, bn2Vect v2) {
    bn2Vect out;

    out.x = v1.x + v2.x;
    out.y = v1.y + v2.y;

    return out;
}

/*
 * function bn3VectSum
 * 
 * Returns v1 + v2.
 */
bn3Vect bn3VectSum(bn3Vect v1, bn3Vect v2) {
    bn3Vect out;

    out.x = v1.x + v2.x;
    out.y = v1.y + v2.y;
    out.z = v1.z + v2.z;

    return out;
}

/*
 * function bn2VectAdd
 * 
 * Adds the second vector to the first vector.
 */
void bn2VectAdd(bn2Vect *v1, bn2Vect v2) {
    (*v1).x += v2.x;
    (*v1).y += v2.y;
}

/*
 * function bn3VectAdd
 * 
 * Adds the second vector to the first vector.
 */
void bn3VectAdd(bn3Vect *v1, bn3Vect v2) {
    (*v1).x += v2.x;
    (*v1).y += v2.y;
    (*v1).z += v2.z;
}

/*
 * function bn2VectDifference
 * 
 * Returns v1 - v2.
 */
bn2Vect bn2VectDifference(bn2Vect v1, bn2Vect v2) {
    bn2Vect out;

    out.x = v1.x - v2.x;
    out.y = v1.y - v2.y;

    return out;
}

/*
 * function bn3VectDifference
 * 
 * Returns v1 - v2.
 */
bn3Vect bn3VectDifference(bn3Vect v1, bn3Vect v2) {
    bn3Vect out;

    out.x = v1.x - v2.x;
    out.y = v1.y - v2.y;
    out.z = v1.z - v2.z;

    return out;
}

/*
 * function bn2VectSubtract
 * 
 * Subtracts the second vector from the first vector.
 */
void bn2VectSubtract(bn2Vect *v1, bn2Vect v2) {
    (*v1).x -= v2.x;
    (*v1).y -= v2.y;
}

/*
 * function bn3VectSubtract
 * 
 * Subtracts the second vector from the first vector.
 */
void bn3VectSubtract(bn3Vect *v1, bn3Vect v2) {
    (*v1).x -= v2.x;
    (*v1).y -= v2.y;
    (*v1).z -= v2.z;
}

/*
 * function bn2VectProduct
 * 
 * Returns v * s.
 */
bn2Vect bn2VectProduct(bn2Vect v, BRAZEN_DTYPE s) {
    bn2Vect out;

    out.x = v.x * s;
    out.y = v.y * s;

    return out;
}

/*
 * function bn3VectProduct
 * 
 * Returns v * s.
 */
bn3Vect bn3VectProduct(bn3Vect v, BRAZEN_DTYPE s) {
    bn3Vect out;

    out.x = v.x * s;
    out.y = v.y * s;
    out.z = v.z * s;

    return out;
}

/*
 * function bn2VectMultiply
 * 
 * Multiplies the vector by the scalar.
 */
void bn2VectMultiply(bn2Vect *v, BRAZEN_DTYPE s) {
    (*v).x *= s;
    (*v).y *= s;
}

/*
 * function bn3VectMultiply
 * 
 * Multiplies the vector by the scalar.
 */
void bn3VectMultiply(bn3Vect *v, BRAZEN_DTYPE s) {
    (*v).x *= s;
    (*v).y *= s;
    (*v).z *= s;
}

/*
 * function bn2VectQuotient
 * 
 * Returns v / s.
 */
bn2Vect bn2VectQuotient(bn2Vect v, BRAZEN_DTYPE s) {
    bn2Vect out;
    s = 1. / s;

    out.x = v.x * s;
    out.y = v.y * s;

    return out;
}

/*
 * function bn3VectQuotient
 * 
 * Returns v / s.
 */
bn3Vect bn3VectQuotient(bn3Vect v, BRAZEN_DTYPE s) {
    bn3Vect out;
    s = 1. / s;

    out.x = v.x * s;
    out.y = v.y * s;
    out.z = v.z * s;

    return out;
}

/*
 * function bn2VectDivide
 * 
 * Divides the vector by the scalar.
 */
void bn2VectDivide(bn2Vect *v, BRAZEN_DTYPE s) {
    s = 1. / s;

    (*v).x *= s;
    (*v).y *= s;
}

/*
 * function bn3VectDivide
 * 
 * Divides the vector by the scalar.
 */
void bn3VectDivide(bn3Vect *v, BRAZEN_DTYPE s) {
    s = 1. / s;

    (*v).x *= s;
    (*v).y *= s;
    (*v).z *= s;
}

/*
 * function bn2VectUnit
 * 
 * Scales v to have magnitude of 1.
 * 
 * If v is the zero vector, does not modify v.
 * 
 * Returns BRAZEN_FAILURE if v is the zero vector, otherwise BRAZEN_SUCCESS.
 */
int bn2VectUnit(bn2Vect *v) {
    BRAZEN_DTYPE magnitude_squared = bn2VectMagnitudeSquared(*v);
    
    if (magnitude_squared != 0.) {
        bn2VectDivide(v, BRAZEN_SQRT(magnitude_squared));
        return BRAZEN_SUCCESS;
    }
    else {
        return BRAZEN_FAILURE;
    }
}

/*
 * function bn3VectUnit
 * 
 * Scales v to have magnitude of 1.
 * 
 * If v is the zero vector, does not modify v.
 * 
 * Returns BRAZEN_FAILURE if v is the zero vector, otherwise BRAZEN_SUCCESS.
 */
int bn3VectUnit(bn3Vect *v) {
    BRAZEN_DTYPE magnitude_squared = bn3VectMagnitudeSquared(*v);
    
    if (magnitude_squared != 0.) {
        bn3VectDivide(v, BRAZEN_SQRT(magnitude_squared));
        return BRAZEN_SUCCESS;
    }
    else {
        return BRAZEN_FAILURE;
    }
}

/*
 * function bn2VectDot
 * 
 * Returns the dot product of the two vectors.
 */
BRAZEN_DTYPE bn2VectDot(bn2Vect v1, bn2Vect v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/*
 * function bn3VectDot
 * 
 * Returns the dot product of the two vectors.
 */
BRAZEN_DTYPE bn3VectDot(bn3Vect v1, bn3Vect v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/*
 * function bn2VectMagnitudeSquared
 * 
 * Returns the squared magnitude of the vector.
 */
BRAZEN_DTYPE bn2VectMagnitudeSquared(bn2Vect v) {
    return v.x * v.x + v.y * v.y;
}

/*
 * function bn3VectMagnitudeSquared
 * 
 * Returns the squared magnitude of the vector.
 */
BRAZEN_DTYPE bn3VectMagnitudeSquared(bn3Vect v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/*
 * function bn2VectMagnitude
 * 
 * Returns the magnitude of the vector.
 */
BRAZEN_DTYPE bn2VectMagnitude(bn2Vect v) {
    return BRAZEN_SQRT(bn2VectMagnitudeSquared(v));
}

/*
 * function bn3VectMagnitude
 * 
 * Returns the magnitude of the vector.
 */
BRAZEN_DTYPE bn3VectMagnitude(bn3Vect v) {
    return BRAZEN_SQRT(bn3VectMagnitudeSquared(v));
}
