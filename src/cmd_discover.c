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

#include <argp.h>
#include <stdlib.h>

#include "argp_utils.h"
#include "discover.h"


static char doc[] = "Send a PS4 discovery request.";

static struct argp_option options[] = {
		{ "ip", 'i', "IP", 0, "IP to send discovery request to", 0 },
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

static struct argp argp = { options, parse_opt, 0, doc, 0, 0, 0 };

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