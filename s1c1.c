// Convert hex to base64.
//
// This program takes as an argument a hex string, which it converts to the
// base64 encoding and then prints to stdout.

#include "bytes.h"
#include "commandline.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


// `base64_digits` contains the digits used in base64, in order.
static const char base64_digits[64] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char padding = '=';


// Print a usage message to stderr.
static void print_usage(const char *const argv[])
{
    const char *name = program_name(argv, "s1c1");
    fprintf(stderr, "Usage: %s hexstring\n", name);
}

// Print the base64 representation of a bytestring.
static void print_as_base64(const bytestring *bytes)
{
    // Base64 uses 4 characters to represent 3 bytes.
    size_t num_triplets = bytes->len / 3 + (bytes->len % 3 != 0);
    for (size_t i = 0; i < num_triplets; i++) {
        // Store the triplet, made up of bytes A, B and C, in the lower 24 bits
        // of a single integer.  Byte A is always valid and used for
        // determining base64 digits.  Bytes B and C are only used if they
        // don't lie beyond the end of the hex string.
        bool use_b = 3 * i + 1 < bytes->len;
        bool use_c = 3 * i + 2 < bytes->len;
        uint_fast32_t byte_a = bytes->data[3 * i + 0];
        uint_fast32_t byte_b = use_b ? bytes->data[3 * i + 1] : 0;
        uint_fast32_t byte_c = use_c ? bytes->data[3 * i + 2] : 0;
        uint_fast32_t value = byte_a << 16 | byte_b << 8 | byte_c;

        // Extract 6 bits for each of the 4 base64 characters.
        char base64[4];
        unsigned char mask = (1 << 6) - 1;
        base64[0] = base64_digits[value >> 18 & mask];
        base64[1] = base64_digits[value >> 12 & mask];
        base64[2] = use_b ? base64_digits[value >> 6 & mask] : padding;
        base64[3] = use_c ? base64_digits[value & mask] : padding;
        printf("%.4s", base64);
    }
    printf("\n");
}

// Print the base64 representation of a hex string and return true on success.
static bool print_base64_from_hex(const char *hex)
{
    bytestring *bytes = decode_hex(hex);
    if (bytes == NULL) {
        return false;
    }

    print_as_base64(bytes);
    free_bytestring(bytes);
    return true;
}

// Print the base64 representation of the hex string given as an argument.
int main(int argc, const char *const argv[])
{
    if (argc != 2) {
        print_usage(argv);
        return EXIT_FAILURE;
    }
    if (!print_base64_from_hex(argv[1])) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
