#include "../bytes.h"
#include "../cryptanalysis.h"
#include "unittest.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


static bool check_frequencies(const char *program_name)
{
    for (int byte = 0; byte < 256; byte++) {
        bool is_printable = is_printable_ascii((unsigned char)byte);
        double log_freq = english_log_frequency((unsigned char)byte);
        bool passed = is_printable ? log_freq <= 0.0 : log_freq == -HUGE_VAL;
        if (!passed) {
            print_failure_context(program_name, "check_frequencies");
            printf("english_log_frequency(%d) should be %s\n", byte,
                   is_printable ? "<= 0.0" : "-HUGE_VAL");
            return false;
        }
    }
    return true;
}

static void test_frequency_sum(tests_summary *summary)
{
    double sum = 0.0;
    for (int byte = 0; byte < 256; byte++) {
        sum += exp(english_log_frequency((unsigned char)byte));
    }
    test_that(fabs(sum - 1.0) <= 2e-7, summary,
              "sum of character frequencies should be close to 1");
}

static bool check_space_common(const char *program_name)
{
    // Use 32 rather than ' ' so that this test works regardless of the
    // machine's encoding.
    double space_log_freq = english_log_frequency(32);
    for (unsigned char byte = 33; byte <= last_printable_ascii; byte++) {
        if (english_log_frequency(byte) >= space_log_freq) {
            print_failure_context(program_name, "check_space_most_common");
            printf("byte %d should have lower frequency than space\n", byte);
            return false;
        }
    }
    return true;
}

static bool check_lowercase_e_common(const char *program_name)
{
    // Use 101 rather than 'e' so that this test works regardless of the
    // machine's encoding.
    unsigned char lowercase_e = 101;
    double lowercase_e_log_freq = english_log_frequency(lowercase_e);
    for (unsigned char byte = 33; byte <= last_printable_ascii; byte++) {
        if (byte == lowercase_e) {
            continue;
        }
        if (english_log_frequency(byte) >= lowercase_e_log_freq) {
            print_failure_context(program_name, "check_lowercase_e_common");
            printf("byte %d should have lower frequency than 'e'\n", byte);
            return false;
        }
    }
    return true;
}

int main(void)
{
    tests_summary summary = {.name = "cryptanalysis"};

    run_test(check_frequencies, &summary);
    test_frequency_sum(&summary);
    run_test(check_space_common, &summary);
    run_test(check_lowercase_e_common, &summary);

    print_summary(&summary);
    if (summary.failed > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
