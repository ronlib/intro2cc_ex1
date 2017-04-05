#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "connect_socket.h"
#include "bit_serializer.h"
#include "hamming.h"

#define FILE_BUFFER_SIZE 63

int main(int argc, const char* argv[])
{
	int socket = 0;
	unsigned char file_buffer[FILE_BUFFER_SIZE], *read_bits = 0, *decoded_bits = 0, *decoded_bytes = 0;
	FILE *file = 0;
	size_t nread;
	int return_code = 0, num_bits = 0, num_bits2 = 0, num_bytes = 0,
		hamming_decode_result;
	unsigned int corrected_bits = 0, received_bytes = 0, wrote_bytes = 0;

	if (argc < 4)
		{
			fprintf(stderr, "Not enough arguments\n");
			return_code = 1;
			goto cleanup;
		}

	socket = connect_to_server(argv[1], argv[2]);
	if (-1 == socket)
		{
			fprintf(stderr, "Unable to connect to channel. Exiting.\n");
			return_code = 1;
			goto cleanup;
		}

	file = fopen(argv[3], "w");
	if (NULL == file)
		{
			fprintf(stderr, "Unable to open file. Exiting.\n");
			return_code = 1;
			goto cleanup;
		}

	while ((nread = recv(socket, file_buffer, sizeof(file_buffer), 0)) > 0)
		{
			while (nread < FILE_BUFFER_SIZE)
				{
					int recv_size = recv(socket, file_buffer+FILE_BUFFER_SIZE-nread, FILE_BUFFER_SIZE-nread, 0);
					nread += recv_size;

					if (nread != FILE_BUFFER_SIZE && recv_size < 1)
						{
							fprintf(stderr, "Error: Should have received a multiple of %d bytes, but got only %lu. Exiting.\n", FILE_BUFFER_SIZE, nread);
							return_code = 1;
							goto cleanup;
						}
				}

			received_bytes += nread;

			if (-1 == bytes2bits(file_buffer, nread, &read_bits, &num_bits))
				{
					return_code = 1;
					goto cleanup;
				}

			hamming_decode_result = hamming_decode(read_bits, num_bits, &decoded_bits, &num_bits2);
			if (-1 == hamming_decode_result)
				{
					fprintf(stderr, "Error while decoding hamming encoded buffer\n");
					return_code = 1;
					goto cleanup;
				}

			corrected_bits += hamming_decode_result;

			if (-1 == bits2bytes(decoded_bits, num_bits2, &decoded_bytes, &num_bytes))
				{
					fprintf(stderr, "Error: Unable to transform bit array to bytes\n");
					return_code = 1;
					goto cleanup;
				}

			size_t nwrite = fwrite(decoded_bytes, 1, num_bytes, file);
			wrote_bytes += num_bytes;

		}

	fprintf(stderr, "%-15s %d bytes\n%-15s %d bytes\n%-15s %d errors\n", "received:",
					received_bytes, "wrote:", wrote_bytes, "corrected:", corrected_bits);

	send(socket, &received_bytes, sizeof(received_bytes), 0);
	send(socket, &wrote_bytes, sizeof(wrote_bytes), 0);
	send(socket, &corrected_bits, sizeof(corrected_bits), 0);

 cleanup:
	if (socket > 0)
		{
			close(socket);
		}

	if (NULL != file)
		{
			fclose(file);
		}

	if (0 != read_bits)
		{
			free(read_bits);
		}

	if (0 != decoded_bits)
		{
			free(decoded_bits);
		}

	if (0 != decoded_bytes)
		{
			free(decoded_bytes);
		}

	return return_code;
}
