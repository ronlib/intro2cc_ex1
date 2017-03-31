#include <stdio.h>
#include "bit_serializer.h"

int main()
{
	unsigned char char_array[] =
		{0xff, 0x00, 0x33};

	unsigned char bits_array[] =
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01};

	unsigned int bytes_length = 0;
	unsigned char * bytes_array = 0;

	printf("bit %d value: %d\n", 3, get_bit_in_byte_array(char_array, 3));

	bits2bytes(bits_array, sizeof(bits_array),

	return 0;
}
