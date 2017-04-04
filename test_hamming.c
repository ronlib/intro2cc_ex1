#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hamming.h"
#include "bit_serializer.h"

int main()
{
	unsigned char test[] = {0x36, 0x56, 0xe2, 0x14, 0xc9, 0x81, 0x49};
	unsigned char *bits = 0, bits2[57], *encoded_hamming = 0, *decoded_hamming = 0;
	unsigned int bytes_length, decode_ret_val = 0, bits_length = 0,
		encoded_hamming_length = 0, decoded_hamming_length = 0;

	unsigned char random1[] = {
		0xd0, 0x31, 0x4e, 0x81, 0xaa, 0xf1, 0x2f, 0x70, 0xa5, 0x8d, 0x95, 0x43,
		0x1d, 0x7a, 0x7b, 0x75, 0xb3, 0xef, 0x57, 0x1c, 0x5e, 0x90, 0x3a, 0xbd,
		0xc3, 0xf5, 0x8b, 0x0a, 0x9f, 0x1f, 0x0a, 0x60, 0x26, 0xc6, 0x4f, 0x9d,
		0xeb, 0x19, 0xce, 0x44, 0x94, 0x4d, 0xdf, 0x4d, 0x50, 0xb5, 0xd6, 0xc6,
		0xb8, 0xe3, 0x25, 0x97, 0x87, 0x7e, 0x84, 0x96, 0x9c
	};

	printf("sizeof(random1) = %lu\n", sizeof(random1));
	bytes2bits(random1, sizeof(random1), &bits, &bits_length);
	//	memcpy(bits2, bits, 56);
	//	bits2[56] = 1;

	hamming_encode(bits, bits_length, &encoded_hamming, &encoded_hamming_length);
	hamming_decode(encoded_hamming, encoded_hamming_length, &decoded_hamming, &decoded_hamming_length);

	if (memcmp(bits, decoded_hamming, bits_length) != 0)
		{
			fprintf(stderr, "Error: Original and decoded hamming bits are different!\n");
		}

	free(bits);
	free(encoded_hamming);
	free(decoded_hamming);


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
