#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


int connect_to_server(const char* ip_address, const char* port_str)
{

	int channel_socket = 0;
	struct sockaddr_in channel_addr = {0};
	int err = 0;
	int port = 0;
	char * strend = 0;


	err = inet_pton(AF_INET, ip_address, &(channel_addr.sin_addr));

	if (0 == err)
		{
			fprintf(stderr, "The given argument %s is not an IP address!\n", ip_address);
			return -1;
		}
	else if (-1 == err)
		{
			fprintf(stderr, "Error in inet_pton()\n");
			return -1;
		}

	port = strtol(port_str, &strend, 10);
	channel_socket = socket(AF_INET, SOCK_STREAM, 0);
	channel_addr.sin_family = AF_INET;
	channel_addr.sin_port = htons(port);
	channel_socket = socket(AF_INET, SOCK_STREAM, 0);

	err = connect(channel_socket, (struct sockaddr*)&channel_addr, sizeof(channel_addr));
	if (0 != err)
		{
			fprintf(stderr, "Unable to connect to remote host\n");
			return -1;
		}

	fprintf(stderr, "Connected successfuly to %s:%s\n", ip_address, port_str);

	return channel_socket;
}
