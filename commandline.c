#include "commandline.h"

#include <stdio.h>


void print_usage(const char *const argv[], const char *default_name,
                 const char *expected_args)
{
    const char *name = program_name(argv, default_name);
    if (expected_args[0] == '\0') {
        fprintf(stderr, "Usage: %s\n", name);
    } else {
        fprintf(stderr, "Usage: %s %s\n", name, expected_args);
    }
}

const char *program_name(const char *const argv[], const char *default_name)
{
    if (argv[0] != NULL && argv[0][0] != '\0') {
        return argv[0];
    }
    return default_name;
}
