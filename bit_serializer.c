#include "bit_serializer.h"

/* int bits2bytes(__IN__ const char* bits, __IN__ unsigned int bits_length, __OUT__ unsigned char* bytes, __OUT__ unsigned int* bytes_length) */
/* { */

/* } */

char get_bit_in_byte_array(__IN__ const unsigned char* bytes, __IN__ unsigned int bit_index)
{
	return bytes[bit_index/8]>>7-(bit_index%8) & 1;
}
