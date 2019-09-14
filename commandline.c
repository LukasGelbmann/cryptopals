#include "commandline.h"

#include <stddef.h>


const char *program_name(const char *const argv[], const char *default_name)
{
    if (argv[0] != NULL && argv[0][0] != '\0') {
        return argv[0];
    }
    return default_name;
}
