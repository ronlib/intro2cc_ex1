#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc, const char* argv[])
{
	long int client_listening_port = 0;
	long int server_listening_port = 0;
	double error_probability = 0;
	long int seed = 0;
	char * strend = 0;

	if (argc < 5)
		{
			fprintf(stderr, "Not enough arguments\n");
			return 1;
		}

	client_listening_port = strtol(argv[1], &strend, 10);
	server_listening_port = strtol(argv[2], &strend, 10);
	error_probability = atof(argv[3]);
	seed = strtol(argv[4], &strend, 10);

	return 0;
}
