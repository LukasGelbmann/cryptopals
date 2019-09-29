#include "../bytes.h"
#include "unittest.h"

#include <stdlib.h>


static void run_error_tests(tests_summary *summary)
{
    int old_stderr_fd = discard_stderr();
    bytestring *bytes;

    bytes = decode_hex("fake");
    test_that(bytes == NULL, summary,
              "decode_hex() should return NULL on invalid hex digit");
    free_bytestring(bytes);

    bytes = decode_hex("bee");
    test_that(bytes == NULL, summary,
              "decode_hex() should return NULL on odd number of hex digits");
    free_bytestring(bytes);

    restore_stderr(old_stderr_fd);
}

static void run_decoding_tests(tests_summary *summary)
{
    bytestring *bytes;

    bytes = decode_hex("");
    test_that(bytes != NULL && bytes->len == 0, summary,
              "decode_hex(\"\") should return an empty bytestring");
    free_bytestring(bytes);

    bytes = decode_hex("00ff05");
    test_that(bytes != NULL && bytes->len == 3 && bytes->data[0] == 0x00
                  && bytes->data[1] == 0xff && bytes->data[2] == 0x05,
              summary, "decode_hex() should return the correct bytestring");
    free_bytestring(bytes);
}

int main(void)
{
    tests_summary summary = {.name = "bytes"};

    free_bytestring(NULL);
    count_passed_test(&summary);

    run_error_tests(&summary);
    run_decoding_tests(&summary);

    print_summary(&summary);
    if (summary.failed > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
