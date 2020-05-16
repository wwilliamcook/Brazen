/*
 * vector_ops.h
 * 
 * Defines bn2Vect, bn3Vect, and helper functions for performing mathematical
 * vector operations.
 */

#ifndef BRAZEN_VECTOR_OPS_H
#define BRAZEN_VECTOR_OPS_H

#include "globals.h"

// Allow linkage with C++ code
#ifdef __cplusplus
extern "C" {
#endif


/*
 * struct bn2Vect
 * 
 * 2-dimensional vector
 */
typedef struct {
    BRAZEN_DTYPE x, y;
} bn2Vect;

/*
 * struct bn3Vect
 * 
 * 3-dimensional vector
 */
typedef struct {
    BRAZEN_DTYPE x, y, z;
} bn3Vect;


/*************************** FUNCTION DECLARATIONS ***************************/

/*
 * function bn2VectZeros
 * 
 * Returns a new zero vector.
 */
bn2Vect bn2VectZeros(void);

/*
 * function bn3VectZeros
 * 
 * Returns a new zero vector.
 */
bn3Vect bn3VectZeros(void);

/*
 * function bn2VectSetZero
 * 
 * Sets all components of the given vector to zero.
 */
void bn2VectSetZero(bn2Vect *v);

/*
 * function bn3VectSetZero
 * 
 * Sets all components of the given vector to zero.
 */
void bn3VectSetZero(bn3Vect *v);

/*
 * function bn2VectSum
 * 
 * Returns v1 + v2.
 */
bn2Vect bn2VectSum(bn2Vect v1, bn2Vect v2);

/*
 * function bn3VectSum
 * 
 * Returns v1 + v2.
 */
bn3Vect bn3VectSum(bn3Vect v1, bn3Vect v2);

/*
 * function bn2VectAdd
 * 
 * Adds the second vector to the first vector.
 */
void bn2VectAdd(bn2Vect *v1, bn2Vect v2);

/*
 * function bn3VectAdd
 * 
 * Adds the second vector to the first vector.
 */
void bn3VectAdd(bn3Vect *v1, bn3Vect v2);

/*
 * function bn2VectDifference
 * 
 * Returns v1 - v2.
 */
bn2Vect bn2VectDifference(bn2Vect v1, bn2Vect v2);

/*
 * function bn3VectDifference
 * 
 * Returns v1 - v2.
 */
bn3Vect bn3VectDifference(bn3Vect v1, bn3Vect v2);

/*
 * function bn2VectSubtract
 * 
 * Subtracts the second vector from the first vector.
 */
void bn2VectSubtract(bn2Vect *v1, bn2Vect v2);

/*
 * function bn3VectSubtract
 * 
 * Subtracts the second vector from the first vector.
 */
void bn3VectSubtract(bn3Vect *v1, bn3Vect v2);

/*
 * function bn2VectProduct
 * 
 * Returns v * s.
 */
bn2Vect bn2VectProduct(bn2Vect v, BRAZEN_DTYPE s);

/*
 * function bn3VectProduct
 * 
 * Returns v * s.
 */
bn3Vect bn3VectProduct(bn3Vect v, BRAZEN_DTYPE s);

/*
 * function bn2VectMultiply
 * 
 * Multiplies the vector by the scalar.
 */
void bn2VectMultiply(bn2Vect *v, BRAZEN_DTYPE s);

/*
 * function bn3VectMultiply
 * 
 * Multiplies the vector by the scalar.
 */
void bn3VectMultiply(bn3Vect *v, BRAZEN_DTYPE s);

/*
 * function bn2VectQuotient
 * 
 * Returns v / s.
 */
bn2Vect bn2VectQuotient(bn2Vect v, BRAZEN_DTYPE s);

/*
 * function bn3VectQuotient
 * 
 * Returns v / s.
 */
bn3Vect bn3VectQuotient(bn3Vect v, BRAZEN_DTYPE s);

/*
 * function bn2VectDivide
 * 
 * Divides the vector by the scalar.
 */
void bn2VectDivide(bn2Vect *v, BRAZEN_DTYPE s);

/*
 * function bn3VectDivide
 * 
 * Divides the vector by the scalar.
 */
void bn3VectDivide(bn3Vect *v, BRAZEN_DTYPE s);

/*
 * function bn2VectUnit
 * 
 * Scales v to have magnitude of 1.
 * 
 * If v is the zero vector, does not modify v.
 * 
 * Returns BRAZEN_FAILURE if v is the zero vector, otherwise BRAZEN_SUCCESS.
 */
int bn2VectUnit(bn2Vect *v);

/*
 * function bn3VectUnit
 * 
 * Scales v to have magnitude of 1.
 * 
 * If v is the zero vector, does not modify v.
 * 
 * Returns BRAZEN_FAILURE if v is the zero vector, otherwise BRAZEN_SUCCESS.
 */
int bn3VectUnit(bn3Vect *v);

/*
 * function bn2VectDot
 * 
 * Returns the dot product of the two vectors.
 */
BRAZEN_DTYPE bn2VectDot(bn2Vect v1, bn2Vect v2);

/*
 * function bn3VectDot
 * 
 * Returns the dot product of the two vectors.
 */
BRAZEN_DTYPE bn3VectDot(bn3Vect v1, bn3Vect v2);

/*
 * function bn2VectMagnitudeSquared
 * 
 * Returns the squared magnitude of the vector.
 */
BRAZEN_DTYPE bn2VectMagnitudeSquared(bn2Vect v);

/*
 * function bn3VectMagnitudeSquared
 * 
 * Returns the squared magnitude of the vector.
 */
BRAZEN_DTYPE bn3VectMagnitudeSquared(bn3Vect v);

/*
 * function bn2VectMagnitude
 * 
 * Returns the magnitude of the vector.
 */
BRAZEN_DTYPE bn2VectMagnitude(bn2Vect v);

/*
 * function bn3VectMagnitude
 * 
 * Returns the magnitude of the vector.
 */
BRAZEN_DTYPE bn3VectMagnitude(bn3Vect v);


// Finish extern "C"
#ifdef __cplusplus
}
#endif

#endif
