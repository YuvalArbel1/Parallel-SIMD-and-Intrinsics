#include <immintrin.h>
#include <string.h>
#include "libstr.h"




/**
 * Filters out non-base64 characters from a given string using AVX-512 SIMD instructions.
 *
 * @param str The input string to be filtered.
 * @param filtered The output buffer where the filtered string will be stored.
 */
void avx512_filter_base64(const char* str, char* filtered) {
    int k = 0; // Index for writing to the filtered string
    int len = strlen(str); // Length of the input string

    for (int i = 0; i < len; i += 64) {
        int chunk_size = len - i >= 64 ? 64 : len - i;
        __m512i chunk = _mm512_loadu_si512((__m512i *)(str + i)); // Load chunk

      // Masks for each base64 character range
        __mmask64 mask_AZ = _mm512_cmpge_epi8_mask(chunk, _mm512_set1_epi8('A')) &
                            _mm512_cmple_epi8_mask(chunk, _mm512_set1_epi8('Z'));
        __mmask64 mask_az = _mm512_cmpge_epi8_mask(chunk, _mm512_set1_epi8('a')) &
                            _mm512_cmple_epi8_mask(chunk, _mm512_set1_epi8('z'));
        __mmask64 mask_09 = _mm512_cmpge_epi8_mask(chunk, _mm512_set1_epi8('0')) &
                            _mm512_cmple_epi8_mask(chunk, _mm512_set1_epi8('9'));
        __mmask64 mask_plus = _mm512_cmpeq_epi8_mask(chunk, _mm512_set1_epi8('+'));
        __mmask64 mask_slash = _mm512_cmpeq_epi8_mask(chunk, _mm512_set1_epi8('/'));

        // Combine masks to identify all valid base64 characters
        __mmask64 mask_base64 = mask_AZ | mask_az | mask_09 | mask_plus | mask_slash;

        int num_valid_chars = _mm_popcnt_u64(mask_base64); // Count valid base64 characters

        // Correctly handle the extraction of valid characters
        for (int j = 0, valid_char_count = 0; valid_char_count < num_valid_chars && j < chunk_size; ++j) {
            char current_char = str[i + j];
            if ((mask_base64 >> j) & 1) { // Check if the character at position j is valid
                filtered[k++] = current_char;
                valid_char_count++;
            }
        }
    }

    filtered[k] = '\0'; // Null-terminate the string
}

/**
 * Converts a base64 character to its corresponding numerical value.
 *
 * @param c The base64 character.
 * @return The numerical value of the base64 character.
 */
int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';        // 'A'-'Z' -> 0-25
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;   // 'a'-'z' -> 26-51
    if (c >= '0' && c <= '9') return c - '0' + 52;   // '0'-'9' -> 52-61
    if (c == '+') return 62;                         // '+' -> 62
    if (c == '/') return 63;                         // '/' -> 63
    return -1;  // Invalid character
}

/**
 * Decodes a base64-encoded string into its numeric representation.
 *
 * @param str The base64-encoded string.
 * @return The numeric representation of the base64 string.
 */
int base64_decode(const char* str) {
    int num = 0;
    while (*str) {
        int val = base64_char_value(*str++);
        if (val >= 0) {
            num = (num << 6) | val;
        }
    }
    return num;
}


/**
 * Calculates the base64 distance between two strings. The distance is calculated by
 * decoding the base64 strings into numbers and subtracting the first from the second.
 *
 * @param str1 The first base64-encoded string.
 * @param str2 The second base64-encoded string.
 * @return The base64 distance between the two strings.
 */
int b64_distance(char str1[MAX_STR], char str2[MAX_STR]) {
    char filtered_str1[MAX_STR];
    char filtered_str2[MAX_STR];

    avx512_filter_base64(str1, filtered_str1);
    avx512_filter_base64(str2, filtered_str2);

    int val1 = base64_decode(filtered_str1);
    int val2 = base64_decode(filtered_str2);

    // Prepare SIMD operands
    __m512i simd_val1 = _mm512_set1_epi32(val1);
    __m512i simd_val2 = _mm512_set1_epi32(val2);

    // Perform subtraction
    __m512i result = _mm512_sub_epi32(simd_val2, simd_val1);

    // Extract the result from the SIMD register
    int final_result[16]; // 512-bit register can hold 16 32-bit integers
    _mm512_storeu_si512((__m512i*)final_result, result);

    // The actual distance will be in the first element of the final_result array
    return final_result[0];
}

