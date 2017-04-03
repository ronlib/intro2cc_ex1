#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "connect_socket.h"
#include "hamming.h"
#include "bit_serializer.h"

#define FILE_BUFFER_SIZE 57

int main(int argc, const char* argv[])
{
	int socket;
	char file_buffer[FILE_BUFFER_SIZE];
	FILE *file;
	size_t nread;

	if (argc < 4)
		{
			fprintf(stderr, "Not enough arguments\n");
			return 1;
		}

	socket = connect_to_server(argv[1], argv[2]);
	if (-1 == socket)
		{
			fprintf(stderr, "Unable to connect to channel. Exiting.\n");
			return 1;
		}

	file = fopen(argv[3], "r");
	if (NULL == file)
		{
			fprintf(stderr, "Unable to open file. Exiting.\n");
			return 1;
		}

	while ((nread = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0)
		{
			int left_to_send = nread, out_buffer_length = 0, out_buffer_length2 = 0;
			unsigned char *bits = 0, *hamming_encoded = 0, *hamming_bytes = 0;

			if (FILE_BUFFER_SIZE > nread)
				{
					fprintf(stderr, "Error: Number of bytes read from a file is not a multiple of 57. Exiting.\n");
					return 1;
				}

			if (-1 == bytes2bits(file_buffer, nread, &bits, &out_buffer_length))
				{
					fprintf(stderr, "Error: Unable to convert bytes to bits. Exiting.\n");
					return 1;
				}

			if (-1 == hamming_encode(bits, out_buffer_length, &hamming_encoded, &out_buffer_length2))
				{
					fprintf(stderr, "Error: Unable to encode using hamming. Exiting.\n");
					free(bits);
					return 1;
				}

			free(bits);

			if (-1 == bits2bytes(hamming_encoded, out_buffer_length2, &hamming_bytes, &out_buffer_length))
				{
					fprintf(stderr, "Error: Unable to convert bits to bytes. Exiting.\n");
					free(hamming_encoded);
					return 1;
				}

			free(hamming_encoded);

			while (left_to_send > 0)
				{
					int nsent = send(socket, file_buffer+nread-left_to_send, left_to_send, 0);
					if (nsent < 1)
						{
							fprintf(stderr, "Error while sending buffer. Exiting.\n");
							free(hamming_bytes);
							return 1;
						}
					left_to_send -= nsent;
				}


		}


 cleanup:
	if (socket > 0)
		{
			close(socket);
		}

	if (NULL != file)
		{
			fclose(file);
		}
	return 0;
}
