#include "convention.h"

/*
	encoded_bits - an allocated array, that must be freed by the caller.
	Returns -1 in case of an error, 0 otherwise
*/
int hamming_encode(__IN__ const unsigned char bits[57], __OUT__ unsigned char** encoded_bits);

int hamming_decode(__IN__ const unsigned char bits[63], __OUT__ unsigned char** decoded_bits);
