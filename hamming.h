#include "convention.h"

/*
	encoded_bits - an 63 bytes allocated array, that must be freed by the caller
	Returns -1 in case of an error, 0 otherwise
*/
int hamming_encode_block(__IN__ const unsigned char bits[57], __OUT__ unsigned char** encoded_bits);


int hamming_decode_block(__IN__ const unsigned char bits[63], __OUT__ unsigned char** decoded_bits);


/*
	Encodes a bits buffer with hamming encoding, and returns the encoded buffer.
	The buffer length must be a multiple of 57 bits.

	Returns -1 in case of an error
*/
int hamming_encode(__IN__ const unsigned char* bits_buffer, __IN__ const unsigned int buffer_length, __OUT__ unsigned char** encoded_buffer, unsigned int* encoded_buffer_length);


/*
	Decodes a bits buffer with hamming encoding, and returns the decoded buffer.
	The buffer length must be a multiple of 63 bits.

	Returns -1 in case of an error, otherwise returns the number of flipped bits
*/
int hamming_decode(__IN__ const unsigned char* bits_buffer, __IN__ const unsigned int buffer_length, __OUT__ unsigned char** decoded_buffer, unsigned int* decoded_buffer_length);
