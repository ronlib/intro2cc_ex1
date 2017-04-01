#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

/*
	 Creates a listening socket, and returns its file descriptor.
	 In case of an error, the returned value is -1
 */
int create_listening_socket(int port);

/*
	Accepts on the given socket. Waits until a connection is made to the socket.
	In case of an error, the returned value is -1.
*/
int receive_connection(int socket);

int main(int argc, const char* argv[])
{
	int client_listening_socket = 0, server_listening_socket = 0,
		client_socket = 0, server_socket = 0;
	long int client_listening_port = 0, server_listening_port = 0;
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

	if ((client_listening_socket = create_listening_socket(client_listening_port)) == -1)
		{
			fprintf(stderr, "Cannot create client listening socket. Exiting\n");
			return 1;
		}

	if ((server_listening_socket = create_listening_socket(server_listening_port)) == -1)
		{
			fprintf(stderr, "Cannot create server listening socket. Exiting\n");
			return 1;
		}

	if ((client_socket = receive_connection(client_listening_socket)) == -1)
		{
			fprintf(stderr, "Cannot receive client connection. Exiting\n");
			return 1;
		}

	if ((server_socket = receive_connection(server_listening_socket)) == -1)
		{
			fprintf(stderr, "Cannot receive server connection. Exiting\n");
			return 1;
		}




	if (0 != server_listening_socket)
		{
			close(server_listening_socket);
		}

	if (0 != client_listening_socket)
		{
			close(client_listening_socket);
		}
	return 0;
}


int create_listening_socket(int port)
{
	int listening_socket = 0;
	struct sockaddr_in server_address;

	listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons((uint16_t)port);

	if (bind(listening_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
		{
			fprintf(stderr, "Cannot bind socket on port %d\n", port);
			return -1;
		}
	return listening_socket;
}


int receive_connection(int socket)
{
	int received_connection_socket = 0;
	struct sockaddr_in client_addr = {0};
	socklen_t client_len = 0;
	received_connection_socket = accept(socket, (struct sockaddr*) &client_addr, &client_len);

	if (-1 == received_connection_socket)
		{
			return -1;
		}

	return received_connection_socket;

}
