
#include <argp.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "argp_utils.h"
#include "spoof.h"


static char doc[] = "Respond to discovery requests with a spoofed IPv4-package.";

static struct argp_option options[] = {
		{ "ps4-host", 'h', "HOST-ID", 0, "PS4 Host ID" },
		{ "ip", 'i', "IP", 0, "IP to spoof or send discovery request to" },
		{ 0 }
};

struct arguments
{
	char *ps4_host;
	char *ip;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch(key)
	{
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

int cmd_spoof(struct argp_state *state)
{
	struct arguments arguments = { 0, 0 };
	parse_argp_subcommand(&argp, state, "spoof", &arguments);

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