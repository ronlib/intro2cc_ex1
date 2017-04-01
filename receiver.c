#include <stdio.h>
#include "connect_socket.h"


int main(int argc, const char* argv[])
{
	if (argc < 4)
		{
			fprintf(stderr, "Not enough arguments\n");
			return 1;
		}

	connect_to_server(argv[1], argv[2]);

	return 0;
}
