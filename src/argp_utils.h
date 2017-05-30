//
// Created by florian on 30.05.17.
//

#ifndef PS4_DISCOVERY_ARGP_UTILS_H
#define PS4_DISCOVERY_ARGP_UTILS_H

#include <argp.h>

error_t parse_argp_subcommand(const struct argp *argp, struct argp_state *state, const char *cmd_name, void *input);

#endif //PS4_DISCOVERY_ARGP_UTILS_H
