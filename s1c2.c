// XOR of two bytestrings.
//
// This program takes two hex strings as its arguments and outputs their XOR
// combination.

#include "bytes.h"
#include "commandline.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Modify `bytes` by performing an XOR with `other`.
static void inplace_xor(const bytestring *bytes, const bytestring *other)
{
    for (size_t i = 0; i < bytes->len && i < other->len; i++) {
        bytes->data[i] ^= other->data[i];
    }
}

// Print the XOR combination of two hex strings and return true on success.
static bool print_xor(const char *hex_a, const char *hex_b)
{
    bool success;
    bytestring *bytes_a = decode_hex(hex_a);
    bytestring *bytes_b = decode_hex(hex_b);

    if (bytes_a == NULL || bytes_b == NULL) {
        success = false;
    } else if (bytes_a->len != bytes_b->len) {
        fputs("Error: the bytestrings have different lengths\n", stderr);
        success = false;
    } else {
        inplace_xor(bytes_a, bytes_b);
        print_as_hex(bytes_a);
        success = true;
    }
    free_bytestring(bytes_a);
    free_bytestring(bytes_b);
    return success;
}

// Print the XOR combination of two hex strings given as arguments.
int main(int argc, const char *const argv[])
{
    if (argc != 3) {
        print_usage(argv, "s1c2", "hexstring_a hexstring_b");
        return EXIT_FAILURE;
    }
    if (!print_xor(argv[1], argv[2])) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
