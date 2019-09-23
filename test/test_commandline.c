#include "../commandline.h"
#include "unittest.h"

#include <stdlib.h>


int main(void)
{
    tests_summary summary = {.name = "commandline"};
    const char *result;

    result = program_name((const char *[]){NULL}, "default-1");
    test_str_equals(result, "default-1", "program_name_no_args", &summary);

    result = program_name((const char *[]){"", "a", NULL}, "123");
    test_str_equals(result, "123", "program_name_empty_arg", &summary);

    result = program_name((const char *[]){"./ABC", NULL}, "ABC");
    test_str_equals(result, "./ABC", "program_name_valid_name", &summary);

    print_summary(&summary);
    if (summary.failed > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
