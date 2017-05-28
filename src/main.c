
#include <stdlib.h>
#include <arpa/inet.h>
#include <argp.h>

#include "common.h"
#include "discover.h"
#include "spoof.h"

static char doc[] = "An open source implementation of the PS4 Discovery protocol.";

static struct argp_option options[] = {
		{ "spoof", 's', 0, 0, "Run in spoof mode" },
		{ "discover", 'd', 0, 0, "Run in discovery mode" },
		{ "ps4-host", 'h', "HOST-ID", 0, "PS4 Host ID" },
		{ "ip", 'i', "IP", 0, "IP to spoof or send discovery request to" },
		{ 0 }
	};

struct arguments
{
	int spoof_mode;
	int discover_mode;
	char *ps4_host;
	char *ip;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch(key)
	{
		case 's':
			arguments->spoof_mode = 1;
			break;
		case 'd':
			arguments->discover_mode = 1;
			break;
		case 'h':
			arguments->ps4_host = arg;
			break;
		case 'i':
			arguments->ip = arg;
			break;
		case ARGP_KEY_ARG:
			argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, 0, doc };

static void cleanup()
{
	ps4_discovery_free();
}

int main(int argc, char **argv)
{
	struct arguments arguments;
	arguments.spoof_mode = 0;
	arguments.discover_mode = 0;
	arguments.ip = 0;
	arguments.ps4_host = 0;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if(!ps4_discovery_init())
		return EXIT_FAILURE;

	atexit(cleanup);

	if(arguments.discover_mode + arguments.spoof_mode != 1)
	{
		fprintf(stderr, "Exactly one mode must be selected.\n");
		return EXIT_FAILURE;
	}
	else if(arguments.discover_mode)
	{
		if(!arguments.ip)
		{
			fprintf(stderr, "ip for discovery is not set.\n");
			return EXIT_FAILURE;
		}

		ps4_discover(arguments.ip);
		return EXIT_SUCCESS;
	}
	else if(arguments.spoof_mode)
	{
		if(!arguments.ip)
		{
			fprintf(stderr, "spoof ip is not set.\n");
			return EXIT_FAILURE;
		}

		if(!arguments.ps4_host)
		{
			fprintf(stderr, "PS4 host id is not set.\n");
			return EXIT_FAILURE;
		}

		in_addr_t ps4_ip = inet_addr(arguments.ip);
		if(ps4_ip == INADDR_NONE)
		{
			fprintf(stderr, "failed to parse ip \"%s\".\n", arguments.ip);
			return EXIT_FAILURE;
		}

		return ps4_spoof(arguments.ps4_host, ps4_ip);
	}
}