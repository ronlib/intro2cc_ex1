#include <stdio.h>
#include <assert.h>
#include "bit_serializer.h"

int main()
{
	unsigned char char_array[] =
		{0xff, 0x00, 0x33};

	unsigned char bits_array[] =
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
		 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01,
		0x01};

	unsigned int bytes_length = 0;
	unsigned char * bytes_array = 0;

	assert(get_bit_in_byte_array(char_array, 3) == 1);

	bits2bytes(bits_array, sizeof(bits_array), &bytes_array, &bytes_length);
	assert(bytes_array[0] == 7);
	assert(bytes_array[1] == 35);
	assert(bytes_array[2] == 128);

	printf("all tests passed successfully!\n");
	return 0;
}
