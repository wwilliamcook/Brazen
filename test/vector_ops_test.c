/*
 * vector_ops_test.c
 * 
 * Contains unit tests for the functions defined by vector_ops.h/c
 */

#include "vector_ops.h"

#include <stdlib.h>  // rand
#include <stdio.h>
#include <limits.h>  // RAND_MAX
#include <math.h>  // round, pow


#define ERROR_TOLERANCE 1e-6


/*
 * function: is_close
 * 
 * Returns BRAZEN_TRUE if a and b are within ERROR_TOLERANCE of each other,
 * BRAZEN_FALSE otherwise.
 */
int is_close(BRAZEN_DTYPE a, BRAZEN_DTYPE b);

/*
 * function: is_close_2d
 * 
 * Returns BRAZEN_TRUE if all corresponding components of v1 and v2 are within
 * ERROR_TOLERANCE of each other, BRAZEN_FALSE otherwise.
 */
int is_close_2d(bn2Vect v1, bn2Vect v2);

/*
 * function: is_close_3d
 * 
 * Returns BRAZEN_TRUE if all corresponding components of v1 and v2 are within
 * ERROR_TOLERANCE of each other, BRAZEN_FALSE otherwise.
 */
int is_close_3d(bn3Vect v1, bn3Vect v2);

/*
 * function: randomUniform
 *
 * Returns a random value uniformly distributed on [a, b].
 */
double randomUniform(double a, double b);

/*
 * function: roundDigits
 *
 * Rounds x to d digits.
 */
double roundDigits(double x, int d);


int main(int argc, char* argv[]) {
    bn2Vect a2, b2, c2, d2;
    bn3Vect a3, b3, c3, d3;
    int num_tests = -1, num_fails = -1;
    int result;
    BRAZEN_DTYPE result_float;

    srand(7);

    printf("Beginning unit tests for vector_ops.h/cpp\n");


    //*************************************************************************
    printf("\nTesting function bn2VectZeros:\n");
    //*************************************************************************
    num_tests = 1;
    num_fails = 0;

    // test 1
    a2 = bn2VectZeros();
    b2.x = b2.y = 0.;

    if (!is_close_2d(a2, b2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectZeros:\n");
    //*************************************************************************
    num_tests = 1;
    num_fails = 0;

    // test 1
    a3 = bn3VectZeros();
    b3.x = b3.y = b3.z = 0.;

    if (!is_close_3d(a3, b3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectSetZero:\n");
    //*************************************************************************
    num_tests = 1;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    bn2VectSetZero(&a2);
    b2.x = b2.y = 0.;

    if (!is_close_2d(a2, b2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectSetZero:\n");
    //*************************************************************************
    num_tests = 1;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    bn3VectSetZero(&a3);
    b3.x = b3.y = b3.z = 0.;

    if (!is_close_3d(a3, b3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectSum:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    b2.x = 3.;
    b2.y = 4.;
    c2.x = 4.;
    c2.y = 6.;
    d2 = bn2VectSum(a2, b2);

    if (!is_close_2d(c2, d2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -1.;
    b2.y = -2.;
    c2.x = 0.;
    c2.y = 0.;
    d2 = bn2VectSum(a2, b2);

    if (!is_close_2d(c2, d2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -3.;
    b2.y = -4.;
    c2.x = -2.;
    c2.y = -2.;
    d2 = bn2VectSum(a2, b2);

    if (!is_close_2d(c2, d2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectSum:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = 4.;
    b3.y = 5.;
    b3.z = 6.;
    c3.x = 5.;
    c3.y = 7.;
    c3.z = 9.;
    d3 = bn3VectSum(a3, b3);

    if (!is_close_3d(c3, d3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -1.;
    b3.y = -2.;
    b3.z = -3.;
    c3.x = 0.;
    c3.y = 0.;
    c3.z = 0.;
    d3 = bn3VectSum(a3, b3);

    if (!is_close_3d(c3, d3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -4.;
    b3.y = -5.;
    b3.z = -6.;
    c3.x = -3.;
    c3.y = -3.;
    c3.z = -3.;
    d3 = bn3VectSum(a3, b3);

    if (!is_close_3d(c3, d3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectAdd:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    b2.x = 3.;
    b2.y = 4.;
    c2.x = 4.;
    c2.y = 6.;
    bn2VectAdd(&a2, b2);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -1.;
    b2.y = -2.;
    c2.x = 0.;
    c2.y = 0.;
    bn2VectAdd(&a2, b2);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -3.;
    b2.y = -4.;
    c2.x = -2.;
    c2.y = -2.;
    bn2VectAdd(&a2, b2);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectAdd:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = 4.;
    b3.y = 5.;
    b3.z = 6.;
    c3.x = 5.;
    c3.y = 7.;
    c3.z = 9.;
    bn3VectAdd(&a3, b3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -1.;
    b3.y = -2.;
    b3.z = -3.;
    c3.x = 0.;
    c3.y = 0.;
    c3.z = 0.;
    bn3VectAdd(&a3, b3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -4.;
    b3.y = -5.;
    b3.z = -6.;
    c3.x = -3.;
    c3.y = -3.;
    c3.z = -3.;
    bn3VectAdd(&a3, b3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectDifference:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    b2.x = 3.;
    b2.y = 4.;
    c2.x = -2.;
    c2.y = -2.;
    d2 = bn2VectDifference(a2, b2);

    if (!is_close_2d(c2, d2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -1.;
    b2.y = -2.;
    c2.x = 2.;
    c2.y = 4.;
    d2 = bn2VectDifference(a2, b2);

    if (!is_close_2d(c2, d2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -3.;
    b2.y = -4.;
    c2.x = 4.;
    c2.y = 6.;
    d2 = bn2VectDifference(a2, b2);

    if (!is_close_2d(c2, d2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectDifference:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = 4.;
    b3.y = 5.;
    b3.z = 6.;
    c3.x = -3.;
    c3.y = -3.;
    c3.z = -3.;
    d3 = bn3VectDifference(a3, b3);

    if (!is_close_3d(c3, d3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -1.;
    b3.y = -2.;
    b3.z = -3.;
    c3.x = 2.;
    c3.y = 4.;
    c3.z = 6.;
    d3 = bn3VectDifference(a3, b3);

    if (!is_close_3d(c3, d3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -4.;
    b3.y = -5.;
    b3.z = -6.;
    c3.x = 5.;
    c3.y = 7.;
    c3.z = 9.;
    d3 = bn3VectDifference(a3, b3);

    if (!is_close_3d(c3, d3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectSubtract:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    b2.x = 3.;
    b2.y = 4.;
    c2.x = -2.;
    c2.y = -2.;
    bn2VectSubtract(&a2, b2);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -1.;
    b2.y = -2.;
    c2.x = 2.;
    c2.y = 4.;
    bn2VectSubtract(&a2, b2);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 1.;
    a2.y = 2.;
    b2.x = -3.;
    b2.y = -4.;
    c2.x = 4.;
    c2.y = 6.;
    bn2VectSubtract(&a2, b2);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectSubtract:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = 4.;
    b3.y = 5.;
    b3.z = 6.;
    c3.x = -3.;
    c3.y = -3.;
    c3.z = -3.;
    bn3VectSubtract(&a3, b3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -1.;
    b3.y = -2.;
    b3.z = -3.;
    c3.x = 2.;
    c3.y = 4.;
    c3.z = 6.;
    bn3VectSubtract(&a3, b3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -4.;
    b3.y = -5.;
    b3.z = -6.;
    c3.x = 5.;
    c3.y = 7.;
    c3.z = 9.;
    bn3VectSubtract(&a3, b3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectProduct:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    c2.x = 3.;
    c2.y = 6.;
    d2 = bn2VectProduct(a2, 3);

    if (!is_close_2d(d2, c2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    c2.x = -1.;
    c2.y = -2.;
    d2 = bn2VectProduct(a2, -1.);

    if (!is_close_2d(d2, c2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 3.;
    a2.y = 4.;
    c2.x = 15.;
    c2.y = 20.;
    d2 = bn2VectProduct(a2, 5.);

    if (!is_close_2d(d2, c2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectProduct:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = 3.;
    c3.y = 6.;
    c3.z = 9.;
    d3 = bn3VectProduct(a3, 3);

    if (!is_close_3d(d3, c3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = -1.;
    c3.y = -2.;
    c3.z = -3.;
    d3 = bn3VectProduct(a3, -1.);

    if (!is_close_3d(d3, c3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 3.;
    a3.y = 4.;
    a3.z = 5.;
    c3.x = 15.;
    c3.y = 20.;
    c3.z = 25.;
    d3 = bn3VectProduct(a3, 5.);

    if (!is_close_3d(d3, c3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectMultiply:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    c2.x = 3.;
    c2.y = 6.;
    bn2VectMultiply(&a2, 3);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    c2.x = -1.;
    c2.y = -2.;
    bn2VectMultiply(&a2, -1.);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 3.;
    a2.y = 4.;
    c2.x = 15.;
    c2.y = 20.;
    bn2VectMultiply(&a2, 5.);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectMultiply:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = 3.;
    c3.y = 6.;
    c3.z = 9.;
    bn3VectMultiply(&a3, 3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = -1.;
    c3.y = -2.;
    c3.z = -3.;
    bn3VectMultiply(&a3, -1.);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 3.;
    a3.y = 4.;
    a3.z = 5.;
    c3.x = 15.;
    c3.y = 20.;
    c3.z = 25.;
    bn3VectMultiply(&a3, 5.);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectQuotient:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    c2.x = 1. / 3.;
    c2.y = 2. / 3.;
    d2 = bn2VectQuotient(a2, 3);

    if (!is_close_2d(d2, c2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    c2.x = -1.;
    c2.y = -2.;
    d2 = bn2VectQuotient(a2, -1.);

    if (!is_close_2d(d2, c2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 3.;
    a2.y = 4.;
    c2.x = 3. / 5.;
    c2.y = 4. / 5.;
    d2 = bn2VectQuotient(a2, 5.);

    if (!is_close_2d(d2, c2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectQuotient:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = 1. / 3.;
    c3.y = 2. / 3;
    c3.z = 1.;
    d3 = bn3VectQuotient(a3, 3);

    if (!is_close_3d(d3, c3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = -1.;
    c3.y = -2.;
    c3.z = -3.;
    d3 = bn3VectQuotient(a3, -1.);

    if (!is_close_3d(d3, c3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 3.;
    a3.y = 4.;
    a3.z = 5.;
    c3.x = 3. / 5.;
    c3.y = 4. / 5.;
    c3.z = 1.;
    d3 = bn3VectQuotient(a3, 5.);

    if (!is_close_3d(d3, c3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectDivide:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    c2.x = 1. / 3.;
    c2.y = 2. / 3.;
    bn2VectDivide(&a2, 3);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    c2.x = -1.;
    c2.y = -2.;
    bn2VectDivide(&a2, -1.);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 3.;
    a2.y = 4.;
    c2.x = 3. / 5.;
    c2.y = 4. / 5.;
    bn2VectDivide(&a2, 5.);

    if (!is_close_2d(a2, c2)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectDivide:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = 1. / 3.;
    c3.y = 2. / 3;
    c3.z = 1.;
    bn3VectDivide(&a3, 3);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = -1.;
    c3.y = -2.;
    c3.z = -3.;
    bn3VectDivide(&a3, -1.);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 3.;
    a3.y = 4.;
    a3.z = 5.;
    c3.x = 3. / 5.;
    c3.y = 4. / 5.;
    c3.z = 1.;
    bn3VectDivide(&a3, 5.);

    if (!is_close_3d(a3, c3)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectUnit:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    c2.x = 0.4472135955;
    c2.y = 0.894427191;
    result = bn2VectUnit(&a2);

    if (!(is_close_2d(a2, c2) && result == BRAZEN_SUCCESS)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = -3.;
    a2.y = 4.;
    c2.x = -0.6;
    c2.y = 0.8;
    result = bn2VectUnit(&a2);

    if (!(is_close_2d(a2, c2) && result == BRAZEN_SUCCESS)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 0.;
    a2.y = 0.;
    c2.x = 0.;
    c2.y = 0.;
    result = bn2VectUnit(&a2);

    if (!(is_close_2d(a2, c2) && result == BRAZEN_FAILURE)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectUnit:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    c3.x = 0.267261241912;
    c3.y = 0.534522483825;
    c3.z = 0.801783725737;
    result = bn3VectUnit(&a3);

    if (!(is_close_3d(a3, c3) && result == BRAZEN_SUCCESS)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = -3.;
    a3.y = 4.;
    a3.z = 5.;
    c3.x = -0.424264068712;
    c3.y = 0.565685424949;
    c3.z = 0.707106781187;
    result = bn3VectUnit(&a3);

    if (!(is_close_3d(a3, c3) && result == BRAZEN_SUCCESS)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 0.;
    a3.y = 0.;
    a3.z = 0.;
    c3.x = 0.;
    c3.y = 0.;
    c3.z = 0.;
    result = bn3VectUnit(&a3);

    if (!(is_close_3d(a3, c3) && result == BRAZEN_FAILURE)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectDot:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    b2.x = 3.;
    b2.y = 4.;
    result_float = bn2VectDot(a2, b2);

    if (!is_close(result_float, 11.)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 1.;
    a2.y = 2.;
    b2.x = 2.;
    b2.y = -1.;
    result_float = bn2VectDot(a2, b2);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = -3.;
    a2.y = 4.;
    b2.x = 0.;
    b2.y = 0.;
    result_float = bn2VectDot(a2, b2);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectDot:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = 4.;
    b3.y = 5.;
    b3.z = 6.;
    result_float = bn3VectDot(a3, b3);

    if (!is_close(result_float, 32.)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    b3.x = -2.;
    b3.y = 3.;
    b3.z = -1.;
    result_float = bn3VectDot(a3, b3);

    if (!is_close(result_float, 1.)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = -3.;
    a3.y = 4.;
    a3.z = 5.;
    b3.x = 0.;
    b3.y = 0.;
    b3.z = 0.;
    result_float = bn3VectDot(a3, b3);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectMagnitudeSquared:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    result_float = bn2VectMagnitudeSquared(a2);

    if (!is_close(result_float, 5.)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 3.;
    a2.y = 4.;
    result_float = bn2VectMagnitudeSquared(a2);

    if (!is_close(result_float, 25.)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 0;
    a2.y = 0;
    result_float = bn2VectMagnitudeSquared(a2);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectMagnitudeSquared:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    result_float = bn3VectMagnitudeSquared(a3);

    if (!is_close(result_float, 14.)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 3.;
    a3.y = 4.;
    a3.z = 5.;
    result_float = bn3VectMagnitudeSquared(a3);

    if (!is_close(result_float, 50.)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 0.;
    a3.y = 0.;
    a3.z = 0.;
    result_float = bn3VectMagnitudeSquared(a3);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn2VectMagnitude:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a2.x = 1.;
    a2.y = 2.;
    result_float = bn2VectMagnitude(a2);

    if (!is_close(result_float, 2.2360679775)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a2.x = 3.;
    a2.y = 4.;
    result_float = bn2VectMagnitude(a2);

    if (!is_close(result_float, 5.)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a2.x = 0;
    a2.y = 0;
    result_float = bn2VectMagnitude(a2);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\nTesting function bn3VectMagnitude:\n");
    //*************************************************************************
    num_tests = 3;
    num_fails = 0;

    // test 1
    a3.x = 1.;
    a3.y = 2.;
    a3.z = 3.;
    result_float = bn3VectMagnitude(a3);

    if (!is_close(result_float, 3.74165738677)) {
        num_fails++;
        printf("failed test 1\n");
    }

    // test 2
    a3.x = 3.;
    a3.y = 4.;
    a3.z = 5.;
    result_float = bn3VectMagnitude(a3);

    if (!is_close(result_float, 7.07106781187)) {
        num_fails++;
        printf("failed test 2\n");
    }

    // test 3
    a3.x = 0.;
    a3.y = 0.;
    a3.z = 0.;
    result_float = bn3VectMagnitude(a3);

    if (!is_close(result_float, 0.)) {
        num_fails++;
        printf("failed test 3\n");
    }

    if (num_fails == 0) {
        printf("all %d tests passed\n", num_tests);
    }
    else {
        printf("failed %d/%d tests\n", num_fails, num_tests);
    }
    num_tests = num_fails = -1;


    //*************************************************************************
    printf("\n\nThis concludes the unit tests for vector_ops.h/c\n");
    //*************************************************************************

    return 0;
}


/*
 * function: is_close
 * 
 * Returns BRAZEN_TRUE if a and b are within ERROR_TOLERANCE of each other,
 * BRAZEN_FALSE otherwise.
 */
int is_close(BRAZEN_DTYPE a, BRAZEN_DTYPE b) {
    BRAZEN_DTYPE diff = a - b;
    
    if (diff < 0) {
        diff = -diff;
    }

    if (diff <= ERROR_TOLERANCE) {
        return BRAZEN_TRUE;
    }
    else {
        return BRAZEN_FALSE;
    }
}

/*
 * function: is_close_2d
 * 
 * Returns BRAZEN_TRUE if all corresponding components of v1 and v2 are within
 * ERROR_TOLERANCE of each other, BRAZEN_FALSE otherwise.
 */
int is_close_2d(bn2Vect v1, bn2Vect v2) {
    return is_close(v1.x, v2.x) && is_close(v1.y, v2.y);
}

/*
 * function: is_close_3d
 * 
 * Returns BRAZEN_TRUE if all corresponding components of v1 and v2 are within
 * ERROR_TOLERANCE of each other, BRAZEN_FALSE otherwise.
 */
int is_close_3d(bn3Vect v1, bn3Vect v2) {
    return is_close(v1.x, v2.x) && is_close(v1.y, v2.y) && is_close(v1.z, v2.z);
}

/*
 * function: randomUniform
 *
 * Returns a random value uniformly distributed on [a, b].
 */
double randomUniform(double a, double b) {
    double r = (double)(rand()) / (double)RAND_MAX;  // distributed on [0, 1]
    return r * (b - a) + a;
}

/*
 * function: roundDigits
 *
 * Rounds x to d digits.
 */
double roundDigits(double x, int d) {
    double mult = pow(10, (double)(d));
    return round(x * mult) / mult;
}
