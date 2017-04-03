#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hamming.h"
#include "bit_serializer.h"

int main()
{
	unsigned char test[] = {0x36, 0x56, 0xe2, 0x14, 0xc9, 0x81, 0x49};
	unsigned char *bits = 0, bits2[57], *encoded_hamming = 0, *decoded_hamming = 0;
	unsigned int bytes_length, decode_ret_val = 0;

	bytes2bits(test, 7, &bits, &bytes_length);
	memcpy(bits2, bits, 56);
	bits2[56] = 1;

	hamming_encode_block(bits2, &encoded_hamming);
	hamming_decode_block(encoded_hamming, &decoded_hamming);

	if (memcmp(bits2, decoded_hamming, 57) != 0)
		{
			fprintf(stderr, "Error: Original and decoded hamming bits are different!\n");
		}

	free(decoded_hamming);
	decoded_hamming = 0;

	encoded_hamming[17] ^= 1;
	decode_ret_val = hamming_decode_block(encoded_hamming, &decoded_hamming);

	if (memcmp(bits2, decoded_hamming, 57) != 0)
		{
			fprintf(stderr, "Error: Original and decoded hamming bits are different!\n");
		}

	if (1 == decode_ret_val)
		{
			fprintf(stderr, "Changed one bit, and hamming fixed it\n");
		}


	free(bits);
	free(encoded_hamming);


	fprintf(stderr, "All tests passed!\n");


	return 0;
}
