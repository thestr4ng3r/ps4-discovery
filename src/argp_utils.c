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
#include <memory.h>

#include "argp_utils.h"

error_t parse_argp_subcommand(const struct argp *argp, struct argp_state *state, const char *cmd_name, void *input)
{
	int argc = state->argc - state->next + 1;
	char **argv = &state->argv[state->next - 1];

	size_t l = strlen(state->name) + strlen(cmd_name) + 2;
	argv[0] = malloc(l);
	snprintf(argv[0], l, "%s %s", state->name, cmd_name);

	error_t r = argp_parse(argp, argc, argv, ARGP_IN_ORDER, 0, input);

	free(argv[0]);
	return r;
}