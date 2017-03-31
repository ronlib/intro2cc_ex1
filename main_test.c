#include <stdio.h>
#include "bit_serializer.h"

int main()
{
	unsigned char char_array[] =
		{0xff, 0x00, 0x33};

	printf("bit %d value: %d\n", 3, get_bit_in_byte_array(char_array, 3));
	return 0;
}
