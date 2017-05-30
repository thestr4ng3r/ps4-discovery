
#include <stdlib.h>
#include <arpa/inet.h>
#include <argp.h>
#include <memory.h>

#include "cmd_spoof.h"
#include "cmd_discover.h"

#include "common.h"
#include "discover.h"
#include "spoof.h"

static char doc[] = "An open source implementation of the PS4 Discovery protocol."
		"\v"
		"Supported commands are:\n"
		"  spoof        Respond to discovery requests with a spoofed IPv4-package.\n"
		"  discover     Send a PS4 discovery request.\n";

static struct argp_option options[] = {
		{ 0 }
	};

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	switch(key)
	{
		case ARGP_KEY_ARG:
			if(strcmp(arg, "spoof") == 0)
				exit(cmd_spoof(state));
			else if(strcmp(arg, "discover") == 0)
				exit(cmd_discover(state));
			else
				argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, "[<cmd> [CMD-OPTION...]]", doc };

static void cleanup()
{
	ps4_discovery_free();
}

int main(int argc, char **argv)
{
	if(!ps4_discovery_init())
		return EXIT_FAILURE;

	atexit(cleanup);

	argp_parse(&argp, argc, argv, ARGP_IN_ORDER, 0, 0);

	return 0;
}