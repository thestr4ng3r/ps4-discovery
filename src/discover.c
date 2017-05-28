
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>


#include "common.h"
#include "discover.h"

void ps4_discover(const char *host_name)
{
	struct sockaddr_in server_addr;
	struct hostent *h;

	h = gethostbyname(host_name);
	if(h == 0)
	{
		fprintf(stderr, "failed to get host from name \"%s\".\n", host_name);
		return;
	}

	server_addr.sin_family = (sa_family_t)h->h_addrtype;
	memcpy((char *)&server_addr.sin_addr.s_addr, h->h_addr_list[0], (size_t)h->h_length);
	server_addr.sin_port = htons(PS4_DISCOVERY_PORT);

	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0)
	{
		fprintf(stderr, "failed to create socket\n");
		return;
	}

	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	client_addr.sin_port = htons (0);
	int r = bind(s, (struct sockaddr *)&client_addr, sizeof (client_addr));
	if(r < 0)
	{
		fprintf(stderr, "failed to bind (%s)\n", strerror(errno));
		return;
	}


	const int broadcast = 1;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(int));

	struct ps4_discovery_packet packet;
	packet.cmd = PS4_DISCOVERY_CMD_SRCH;
	packet.protocol_version = PS4_DEVICE_DISCOVERY_PROTOCOL_VERSION;

	char buffer[512];
	int len = ps4_discovery_write_packet(&packet, buffer, sizeof(buffer));
	if(len > sizeof(buffer))
		return;
	ssize_t rc = sendto(s, buffer, (size_t)len+1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

	if(rc < 0)
	{
		fprintf(stderr, "failed to send srch (%s)\n", strerror(errno));
		return;
	}


	while(1)
	{
		memset(buffer, 0, sizeof(buffer));

		struct sockaddr_in client_addr2;
		socklen_t client_addr_len = sizeof(client_addr2);
		ssize_t n = recvfrom(s, buffer, sizeof(buffer)-1, 0, (struct sockaddr *)&client_addr2, &client_addr_len);
		if(n < 0)
		{
			fprintf(stderr, "failed to read from socket.\n");
			return;
		}

		if(n == 0)
			continue;

		buffer[n] = '\0';
		printf("received \"%s\"\n", buffer);
	}

	close(s);
}
