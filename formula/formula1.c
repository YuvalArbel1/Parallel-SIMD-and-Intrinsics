#ifndef FORMULA1
#define FORMULA1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <immintrin.h>
#include <string.h>





/**
 * Computes the cube root of the sum of the square roots of each element
 * in the input array, using AVX-512 SIMD instructions for optimization.
 *
 * @param x Pointer to the input array of floats.
 * @param length The number of elements in the input array.
 * @return The cube root of the sum of square roots of all elements in x.
 */
float sum_and_cuberoot(float *x, int length) {
    // Start with 0.0 in all elements for sum.
    __m512 sum_vec = _mm512_setzero_ps();

    int i = 0;

    // Compute square roots and sum, load 16 float in each iter.
    for (i = 0; i <= length - 16; i += 16) {
        __m512 data = _mm512_loadu_ps(&x[i]);
        __m512 sqrt_data = _mm512_sqrt_ps(data);
        sum_vec = _mm512_add_ps(sum_vec, sqrt_data);
    }


    // Handle remaining elements
    if (i < length) {
        unsigned int remaining_elements = length % 16;
        __mmask16 mask = (1 << remaining_elements) - 1;
        __m512 data = _mm512_maskz_loadu_ps(mask, &x[i]);
        __m512 sqrt_data = _mm512_sqrt_ps(data);
        sum_vec = _mm512_mask_add_ps(sum_vec, mask, sum_vec, sqrt_data);
    }

    // Reduce sum_vec to a single float by addition.
    float sum = _mm512_reduce_add_ps(sum_vec);
    // Compute cube root of the sum
    float cube_root = cbrtf(sum);
    return cube_root;
}


/**
 * Computes the product of ((x_k)^2 + 1) for each element x_k in the input
 * array, using AVX-512 SIMD instructions.
 *
 * @param x Pointer to the input array of floats.
 * @param length The number of elements in the input array.
 * @return The product of ((x_k)^2 + 1) for all elements in x.
 */
float product_calc(float *x, int length) {

    // Start with 1.0 in all elements for mul.
    __m512 prod_vec = _mm512_set1_ps(1.0);

    // Set zmm register packed with 1.
    __m512 ones = _mm512_set1_ps(1);


    int i = 0;
    for (i = 0; i <= length - 16; i += 16) {

        // Load 16 floats from the array to zmm using unaligned load.
        __m512 data = _mm512_loadu_ps(&x[i]);

        // Calculate x_k1^2 ... x_k16^2 as SIMD operation.
        __m512 data_squared = _mm512_mul_ps(data, data);


        // Calculate (x_k^2+1).
        __m512 terms = _mm512_add_ps(data_squared, ones);

        // Calculate ((x_k1^2+1)(x_k17^2+1),...,(x_kn-16^2+1)(x_kn^2+1)).
        prod_vec = _mm512_mul_ps(prod_vec, terms);
    }


    // Handle remaining elements
    if (i < length) {
        unsigned int remaining_elements = length - i;
        __mmask16 mask = (1 << remaining_elements) - 1;

        // Load the remaining elements with a mask
        __m512 data = _mm512_maskz_loadu_ps(mask, &x[i]);

        // Perform the operations using the loaded data
        __m512 data_squared = _mm512_mul_ps(data, data);
        __m512 terms = _mm512_add_ps(data_squared, _mm512_set1_ps(1.0));

        // Multiply with the mask applied to avoid affecting unused lanes
        prod_vec = _mm512_mask_mul_ps(prod_vec, mask, prod_vec, terms);
    }

     // Reduce prod_vec to a single float by multiplication.
    float product = _mm512_reduce_mul_ps(prod_vec);
    return product;
}

/**
 * Calculates the specified mathematical formula using SIMD instructions:
 * sqrt(1 + (sum_(k=1)^n sqrt(x_k))^(1/3) / (product_(k=1)^n((x_k)^2 + 1)))
 *
 * @param x Pointer to the input array of floats.
 * @param length The number of elements in the input array.
 * @return The result of the formula calculation.
 */
float formula1(float *x, unsigned int length) {


    int temp_length = (int) length;

    // Compute the product using formula1
    float product = product_calc(x, temp_length);

    // Compute the sum of square roots and their cube root
    float cube_root_sum = sum_and_cuberoot(x, temp_length);

    // Compute the final result of the formula
    float final_result = sqrtf(1.0f + cube_root_sum / product);
    return final_result;

}


#endif
