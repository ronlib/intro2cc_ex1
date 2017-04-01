#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "connect_socket.h"

#define FILE_BUFFER_SIZE 1000

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
			int left_to_send = nread;
			while (left_to_send > 0)
				{
					int nsent = send(socket, file_buffer+nread-left_to_send, left_to_send, 0);
					if (nsent < 1)
						{
							fprintf(stderr, "Error while sending buffer. Exiting.\n");
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
