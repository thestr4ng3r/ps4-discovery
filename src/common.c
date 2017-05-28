
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "common.h"

static regex_t ps4_regex_srch;
static regex_t ps4_regex_protocol_version;

static char *get_regerror(int errcode, regex_t *compiled)
{
	size_t length = regerror(errcode, compiled, NULL, 0);
	char *buffer = malloc(length);
	regerror(errcode, compiled, buffer, length);
	return buffer;
}

int ps4_discovery_init()
{
	int r;
#define REGCOMP_CHECK(a, b, c) { r = regcomp(a, b, c); if(r != 0) { char *str = get_regerror(r, a); fprintf(stderr, "failed to compile regex \"%s\" (%s)\n", b, str); free(str); return 0; } }

	REGCOMP_CHECK(&ps4_regex_srch, "^SRCH \\* HTTP/1.1.*", 0);
	REGCOMP_CHECK(&ps4_regex_protocol_version, "^device-discovery-protocol-version:\\([^\\r\\n]*\\)\\r*$", REG_NEWLINE);

#undef REGCOMP_CHECK
	return 1;
}

void ps4_discovery_free()
{
	regfree(&ps4_regex_srch);
	regfree(&ps4_regex_protocol_version);
}

int ps4_discovery_parse_packet(struct ps4_discovery_packet *packet, char *str)
{
	packet->protocol_version = 0;

	if(regexec(&ps4_regex_srch, str, 0, 0, 0) == 0)
		packet->cmd = PS4_DISCOVERY_CMD_SRCH;
	else
		return 0;

	int end = 0;
	while(*str != '\n' && *str != '\0')
		str++;
	if(*str == '\0')
		end = 1;
	str++;

	do
	{
		char *line_begin = str;
		if(!end)
		{
			while(*str != '\n' && *str != '\0')
				str++;
			if(*str == '\0')
				end = 1;
			str++;
		}

		regmatch_t matches[2];

		if(regexec(&ps4_regex_protocol_version, line_begin, 2, matches, 0) == 0)
		{
			line_begin[matches[1].rm_eo] = 0;
			packet->protocol_version = line_begin + matches[1].rm_so;
		}
	}
	while(!end);

	return 1;
};

int ps4_discovery_write_packet(struct ps4_discovery_packet *packet, char *buffer, size_t buffer_len)
{
	switch(packet->cmd)
	{
		case PS4_DISCOVERY_CMD_SRCH:
			return snprintf(buffer, buffer_len, "SRCH * HTTP/1.1\ndevice-discovery-protocol-version:%s\n",
					 packet->protocol_version ? packet->protocol_version : PS4_DEVICE_DISCOVERY_PROTOCOL_VERSION);
	}

	return 0;
}