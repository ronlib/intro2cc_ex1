#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bit_serializer.h"

int main()
{
	unsigned char char_array[] =
		{0xff, 0x00, 0x33};

	unsigned char bits_array1[] =
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
		 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,
		0x01};

	unsigned int bytes_length = 0;
	unsigned int bits_length = 0;
	unsigned char * bytes_array1 = 0;
	unsigned char * bits_array2 = 0;

	assert(get_bit_in_byte_array(char_array, 3) == 1);
	bytes2bits(char_array, sizeof(char_array), &bits_array2, &bits_length);
	assert(bits_array2[0] == 1);
	assert(bits_array2[8] == 0);
	assert(bits_array2[18] == 1);

	bits2bytes(bits_array1, sizeof(bits_array1), &bytes_array1, &bytes_length);
	assert(bytes_array1[0] == 7);
	assert(bytes_array1[1] == 35);
	assert(bytes_array1[2] == 128);

	free(bits_array2);
	free(bytes_array1);

	printf("All tests passed successfully!\n");
	return 0;
}
