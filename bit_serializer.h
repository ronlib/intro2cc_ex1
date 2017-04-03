#include "convention.h"

/**
 Returns an allocated array of unsigned char (which requires freeing later)

 return codes:
 0 - success
 1 - unsucces
 */
int bits2bytes(__IN__ const unsigned char* bits, __IN__ unsigned int bits_length, __OUT__ unsigned char** bytes, __OUT__ unsigned int* bytes_length);

int bytes2bits(__IN__ const unsigned char* bytes, __IN__ unsigned int bytes_length, __OUT__ unsigned char** bits, __OUT__ unsigned int* bits_length);

/**
 return codes:
 0,1 - the value of the bit
 -1 - error
 */
char get_bit_in_byte_array(__IN__ const unsigned char* bytes, __IN__ unsigned int bit_index);
