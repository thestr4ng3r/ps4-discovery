/**
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <argp.h>
#include <memory.h>

#include "cmd_spoof.h"
#include "cmd_discover.h"

#include "common.h"

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

static struct argp argp = { options, parse_opt, "[<cmd> [CMD-OPTION...]]", doc, 0, 0, 0 };

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