
#include <argp.h>
#include <stdlib.h>

#include "argp_utils.h"
#include "discover.h"


static char doc[] = "Send a PS4 discovery request.";

static struct argp_option options[] = {
		{ "ip", 'i', "IP", 0, "IP to send discovery request to" },
		{ 0 }
};

struct arguments
{
	char *ip;
};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch(key)
	{
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

int cmd_discover(struct argp_state *state)
{
	struct arguments arguments = { 0 };
	parse_argp_subcommand(&argp, state, "discover", &arguments);

	if(!arguments.ip)
	{
		fprintf(stderr, "ip for discovery is not set.\n");
		return EXIT_FAILURE;
	}

	ps4_discover(arguments.ip);
	return EXIT_SUCCESS;
}