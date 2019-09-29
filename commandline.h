#ifndef COMMANDLINE_H
#define COMMANDLINE_H

// This header defines tools related to the command-line interface.


// Print a usage message to stderr.
void print_usage(const char *const argv[], const char *default_name,
                 const char *expected_args);

// Return the program name.
const char *program_name(const char *const argv[], const char *default_name);

#endif
