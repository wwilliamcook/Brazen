/*
 * globals.h
 * 
 * Defines global constants required for the rest of the Brazen library.
 */

#ifndef BEZIER_GLOBALS_H
#define BEZIER_GLOBALS_H


/***************** CHANGE THESE TO MODIFY THE PHYSICS ENGINE *****************/

/*
 * Define the internal data type used for all computations
 */
#define BRAZEN_DTYPE double

/*
 * Define which square root function to use (sqrtf/sqrt/sqrtl)
 */
#define BRAZEN_SQRT sqrt


/**************************** DO NOT CHANGE THESE ****************************/

enum {
    BRAZEN_FALSE = 0,
    BRAZEN_TRUE = 1,
    BRAZEN_SUCCESS,
    BRAZEN_FAILURE
};

#endif
