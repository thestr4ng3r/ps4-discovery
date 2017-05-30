
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