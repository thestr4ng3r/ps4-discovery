
#include <stdio.h>
#include <errno.h>
#include <memory.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "common.h"
#include "spoof.h"


static const char *awake_response = "HTTP/1.1 200 Ok\n"
		"host-id:%s\n"
		"host-type:PS4\n"
		"host-name:PS4\n"
		"host-request-port:997\n"
		"device-discovery-protocol-version:00020020\n"
		"system-version:04550011\n";

static const char *standby_response = "HTTP/1.1 620 Server Standby\n"
		"host-id:%s\n"
		"host-type:PS4\n"
		"host-name:PS4\n"
		"host-request-port:997\n"
		"device-discovery-protocol-version:00020020\n"
		"system-version:04550011\n";


static void ps4_spoof_respond(struct sockaddr_in client_addr, const char *ps4_host_id, in_addr_t ps4_ip);

int ps4_spoof(const char *ps4_host_id, in_addr_t ps4_ip)
{
	struct sockaddr_in server_addr;

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		fprintf(stderr, "failed to create socket (%s)\n", strerror(errno));
		return EXIT_FAILURE;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PS4_DISCOVERY_PORT);

	const int reuseaddr = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));

	int rc = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(rc < 0)
	{
		fprintf(stderr, "failed to bind to port %d (%s)\n", PS4_DISCOVERY_PORT, strerror(errno));
		return EXIT_FAILURE;
	}

	char buffer[512];
	while(1)
	{
		memset(buffer, 0, sizeof(buffer));

		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr *)&client_addr, &client_addr_len);
		if(n < 0)
		{
			fprintf(stderr, "failed to read from socket.\n");
			return EXIT_FAILURE;
		}

		if(n == 0)
			continue;

		buffer[n] = '\0';
		//printf("received \"%s\"\n", buffer);

		struct ps4_discovery_packet packet;
		if(ps4_discovery_parse_packet(&packet, buffer))
		{
			if(packet.cmd == PS4_DISCOVERY_CMD_SRCH)
			{
				printf("received srch command, sending fake reply.\n");
				ps4_spoof_respond(client_addr, ps4_host_id, ps4_ip);
			}
		}
	}

	close(sockfd);

	return EXIT_SUCCESS;
}

static uint16_t ipv4_checksum(struct iphdr *ip_hdr)
{
	uint64_t sum = 0;
	uint16_t *data = (uint16_t *)ip_hdr;

	int i;
	for(i=0; i<sizeof(ip_hdr)/2; i++)
		sum += data[i];

	sum = (sum >> 16) + (sum & 0xffff);
	return ~((uint16_t)sum);
}

static void ps4_spoof_respond(struct sockaddr_in client_addr, const char *ps4_host_id, in_addr_t ps4_ip)
{
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if(sockfd < 0)
	{
		fprintf(stderr, "failed to create raw socket (%s)\n", strerror(errno));
		return;
	}

	char data[4096];
	struct iphdr *ip_hdr = (struct iphdr *)data;
	struct udphdr *udp_hdr = (struct udphdr *)(data + sizeof(struct iphdr));
	char *payload = data + sizeof(struct iphdr) + sizeof(struct udphdr);
	size_t payload_max_len = sizeof(data) - (sizeof(struct iphdr) + sizeof(struct udphdr));

	int payload_len = snprintf(payload, payload_max_len, awake_response, ps4_host_id);
	if(payload_len >= payload_max_len)
	{
		close(sockfd);
		return;
	}

	size_t total_len = sizeof(struct iphdr) + sizeof(struct udphdr) + (size_t)payload_len;

	ip_hdr->version = 4;
	ip_hdr->ihl = 5;
	ip_hdr->tos = 0;
	ip_hdr->tot_len = htons((uint16_t)total_len);
	ip_hdr->id = htons(1337);
	ip_hdr->frag_off = 0;
	ip_hdr->ttl = 255;
	ip_hdr->protocol = IPPROTO_UDP;
	ip_hdr->check = 0;
	ip_hdr->saddr = ps4_ip;
	ip_hdr->daddr = client_addr.sin_addr.s_addr;
	ip_hdr->check = ipv4_checksum(ip_hdr);

	udp_hdr->source = PS4_DISCOVERY_PORT;
	udp_hdr->dest = client_addr.sin_port;
	udp_hdr->len = htons((uint16_t) (8 + payload_len));
	udp_hdr->check = 0;

	// TODO: udp checksum (PS Vita accepts 0)

	if(sendto(sockfd, data, total_len, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
	{
		fprintf(stderr, "failed to send raw data (%s)\n", strerror(errno));
	}

	close(sockfd);
}