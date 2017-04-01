#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1000

/*
	 Creates a listening socket, and returns its file descriptor.
	 In case of an error, the returned value is -1
 */
int create_listening_socket(int port);

/*
	Accepts on the given socket. Waits until a connection is made to the socket.
	In case of an error, the returned value is -1.
*/
int receive_connection(int socket, struct sockaddr* client_addr);

/*
	Returns -1 in case of an error, 0 otherwise.
*/
int transfer_sender_receiver(int sender_socket, int receiver_socket);



int main(int argc, const char* argv[])
{
	int client_listening_socket = 0, server_listening_socket = 0,
		sender_socket = 0, receiver_socket = 0;
	struct sockaddr sender_addr, receiver_addr;
	char address_buffer[INET_ADDRSTRLEN];
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

	if ((sender_socket = receive_connection(client_listening_socket, &sender_addr)) == -1)
		{
			fprintf(stderr, "Cannot receive client connection. Exiting\n");
			return 1;
		}

	if (address_buffer == inet_ntop(AF_INET, &(((struct sockaddr_in*)&sender_addr)->sin_addr), address_buffer, sizeof(struct sockaddr_in)))
		{
			int port_number = ntohs(((struct sockaddr_in*)(&sender_addr))->sin_port);
			fprintf(stderr, "sender: %s:%d\n", address_buffer, port_number);
		}
	else
		{
			goto cleanup;
		}

	if ((receiver_socket = receive_connection(server_listening_socket, &receiver_addr)) == -1)
		{
			fprintf(stderr, "Cannot receive server connection. Exiting\n");
			return 1;
		}

	if (address_buffer == inet_ntop(AF_INET, &(((struct sockaddr_in*)&receiver_addr)->sin_addr), address_buffer, sizeof(struct sockaddr_in)))
		{
			int port_number = ntohs(((struct sockaddr_in*)(&receiver_addr))->sin_port);
			fprintf(stderr, "receiver: %s:%d\n", address_buffer, port_number);
		}
	else
		{
			goto cleanup;
		}


	if (transfer_sender_receiver(sender_socket, receiver_socket) == -1)
		{
			fprintf(stderr, "Error while transferring files from sender to receiver. Exiting.\n");
		}


 cleanup:

	if (0 != server_listening_socket)
		{
			close(server_listening_socket);
		}

	if (0 != client_listening_socket)
		{
			close(client_listening_socket);
		}

	if (0 != receiver_socket)
		{
			close(sender_socket);
		}

	if (0 != receiver_socket)
		{
			close(receiver_socket);
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

	listen(listening_socket, 1);

	return listening_socket;
}


int receive_connection(int socket, struct sockaddr* client_addr)
{
	int received_connection_socket = 0;
	/* struct sockaddr_in client_addr = {0}; */
	socklen_t client_len = sizeof(client_addr);
	received_connection_socket = accept(socket, client_addr, &client_len);

	fprintf(stderr, "A new connection was made!\n");

	if (-1 == received_connection_socket)
		{
			return -1;
		}

	return received_connection_socket;
}

int transfer_sender_receiver(int sender_socket, int receiver_socket)
{
	char buf[BUFFER_SIZE];
	int nrecv;

	while ((nrecv = recv(sender_socket, buf, sizeof(buf), 0)) > 0)
		{
			int left_to_send = nrecv;
			while (left_to_send > 0)
				{
					int nsent = send(receiver_socket, buf+nrecv-left_to_send, left_to_send, 0);
					if (nsent < 1)
						{
							fprintf(stderr, "Error while sending buffer. Exiting.\n");
							return -1;
						}

					left_to_send -= nsent;
				}
		}

	return 0;
}
