#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "connect_socket.h"

#define FILE_BUFFER_SIZE 1000

int main(int argc, const char* argv[])
{
	int socket = 0;
	char file_buffer[FILE_BUFFER_SIZE];
	FILE *file = 0;
	size_t nread;
	int return_code = 0;

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
			size_t nwrite = fwrite(file_buffer, 1, nread, file);
			if (nwrite != nread)
				{
					fprintf(stderr, "Error while trying to write to file %s. Exiting.\n", argv[3]);
					return_code = 1;
					goto cleanup;
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

	return return_code;
}
