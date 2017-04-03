#include "bit_serializer.h"
#include <stdlib.h>
#include <string.h>


char get_bit_in_byte_array(__IN__ const unsigned char* bytes, __IN__ unsigned int bit_index)
{
	return (bytes[bit_index/8]>>(7-(bit_index%8))) & 1;
}

int bits2bytes(__IN__ const unsigned char* bits, __IN__ unsigned int bits_length, __OUT__ unsigned char** bytes, __OUT__ unsigned int* bytes_length)
{
	unsigned int i = 0;
	*bytes_length = (bits_length+7)/8;
	*bytes = malloc(*bytes_length);
	memset(*bytes, 0, *bytes_length);

	for (i=0 ; i < bits_length ; ++i)
		{
			(*bytes)[i/8] <<= 1;
			(*bytes)[i/8] = ((*bytes)[i/8]^bits[i]);
		}

	// Adding zero padding
	(*bytes)[(i-1)/8] <<= 8 - i%8;

	return 0;
}

int bytes2bits(__IN__ const unsigned char* bytes, __IN__ unsigned int bytes_length, __OUT__ unsigned char** bits, __OUT__ unsigned int* bits_length)
{
	*bits_length = bytes_length*8;
	*bits = malloc(*bits_length);
	for (unsigned int i=0 ; i < *bits_length ; ++i)
		{
			(*bits)[i] = get_bit_in_byte_array(bytes, i);
		}

	return 0;
}
