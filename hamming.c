#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hamming.h"
#include "bit_serializer.h"

int set_encoding_bits_according_to_parity(__IN__ unsigned char encoded_bits[63]);


int hamming_encode_block(__IN__ const unsigned char bits[57], __OUT__ unsigned char** encoded_bits)
{
	int err = 0;
	if (0 == encoded_bits)
		{
			return -1;
		}

	*encoded_bits = malloc(63);
	if (0 == *encoded_bits)
		{
			fprintf(stderr, "Unable to allocate memory.\n");
		}


	(*encoded_bits)[0] = 0;
	(*encoded_bits)[1] = 0;
	(*encoded_bits)[3] = 0;
	(*encoded_bits)[7] = 0;
	(*encoded_bits)[15] = 0;
	(*encoded_bits)[31] = 0;

	(*encoded_bits)[2] = bits[0];
	memcpy(&(*encoded_bits)[4], &bits[1], 3);
	memcpy(&(*encoded_bits)[8], &bits[4], 7);
	memcpy(&(*encoded_bits)[16], &bits[11], 15);
	memcpy(&(*encoded_bits)[32], &bits[26], 31);

	set_encoding_bits_according_to_parity(*encoded_bits);

	return 0;
}

int set_encoding_bits_according_to_parity(__IN__ unsigned char encoded_bits[63])
{
	for (int i=1 ; i <= 63 ; ++i)
		{
			if (1 == encoded_bits[i-1] &&
					(i != 1 && i != 2 && i != 4 && i!= 8 && i != 16 && i != 32))
				{
					if (get_bit_in_byte_array((unsigned char*)&i, 7) == 1)
						{
							encoded_bits[0] ^= 1;
						}

					if (get_bit_in_byte_array((unsigned char*)&i, 6) == 1)
						{
							encoded_bits[1] ^= 1;
						}

					if (get_bit_in_byte_array((unsigned char*)&i, 5) == 1)
						{
							encoded_bits[3] ^= 1;
						}

					if (get_bit_in_byte_array((unsigned char*)&i, 4) == 1)
						{
							encoded_bits[7] ^= 1;
						}

					if (get_bit_in_byte_array((unsigned char*)&i, 3) == 1)
						{
							encoded_bits[15] ^= 1;
						}

					if (get_bit_in_byte_array((unsigned char*)&i, 2) == 1)
						{
							encoded_bits[31] ^= 1;
						}
				}
		}

	return 0;
}


int hamming_decode_block(__IN__ const unsigned char bits[63], __OUT__ unsigned char** decoded_bits)
{
	unsigned char tmp[63], parity_misses = 0;
	int return_val = 0;
	memcpy(tmp, bits, 63);

	set_encoding_bits_according_to_parity(tmp);
	parity_misses = tmp[0] | tmp[1]<<1 | tmp[3]<<2 | tmp[7]<<3 | tmp[15]<<4 | tmp[31]<<5;

	// If parity_misses > 0, it means an error occurred in index represented by parity misses.
	// We can correct at most one bit flip.
	if (parity_misses > 0)
		{
			tmp[parity_misses-1] ^= 1;
			return_val = 1;
		}

	*decoded_bits = malloc(57);
	if (0 == *decoded_bits)
		{
			fprintf(stderr, "Unable to allocate memory\n");
			return -1;
		}

	(*decoded_bits)[0] = tmp[2];
	memcpy(&(*decoded_bits)[1], &tmp[4], 3);
	memcpy(&(*decoded_bits)[4], &tmp[8], 7);
	memcpy(&(*decoded_bits)[11], &tmp[16], 15);
	memcpy(&(*decoded_bits)[26], &tmp[32], 31);

	return return_val;
}


int hamming_encode(__IN__ const unsigned char* bits_buffer, __IN__ const unsigned int buffer_length, __OUT__ unsigned char** encoded_buffer, unsigned int* encoded_buffer_length)
{

	if (0 != buffer_length % 57)
		{
			return -1;
		}

	*encoded_buffer_length = (buffer_length*63)/57;
	*encoded_buffer = malloc(*encoded_buffer_length);

	if (0 == *encoded_buffer)
		{
			fprintf(stderr, "Error: Unable to allocate memory.\n");
			return -1;
		}

	for (unsigned int i = 0 ; i < buffer_length ; i += 57)
		{
			unsigned char* encoded_bits = 0;
			if (-1 == hamming_encode_block(&bits_buffer[i], &encoded_bits))
				{
					fprintf(stderr, "Error: Unable to encode a hamming block.\n");
					return -1;
				}

			memcpy(&((*encoded_buffer)[(i*63)/57]), encoded_bits, 63);
			free(encoded_bits);
			encoded_bits = 0;
		}

	return 0;
}


int hamming_decode(__IN__ const unsigned char* bits_buffer, __IN__ const unsigned int buffer_length, __OUT__ unsigned char** decoded_buffer, unsigned int* decoded_buffer_length)
{
	int flipped_bit_counter = 0;

	if (0 != buffer_length % 63)
		{
			return -1;
		}

	*decoded_buffer_length = (buffer_length*63)/57;
	*decoded_buffer = malloc(*decoded_buffer_length);

	if (0 == *decoded_buffer)
		{
			fprintf(stderr, "Error: Unable to allocate memory.\n");
			return -1;
		}

	for (unsigned int i = 0 ; i < buffer_length ; i += 63)
		{
			unsigned char* decoded_bits = 0;
			int decode_result = hamming_decode_block(&bits_buffer[i], &decoded_bits);
			if (-1 == decode_result)
				{
					fprintf(stderr, "Error: Unable to decode a hamming block.\n");
					return -1;
				}
			flipped_bit_counter += decode_result;

			memcpy(&((*decoded_buffer)[(i*57)/63]), decoded_bits, 57);
			free(decoded_bits);
			decoded_bits = 0;
		}

	return flipped_bit_counter;
}
