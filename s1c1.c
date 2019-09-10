// Convert hex to base64.
//
// This program takes as an argument a hex string, which it converts to the
// base64 encoding and then prints to stdout.

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// CHAR_REPR_SIZE is the number of characters needed for a readable string of a
// single char: e.g., "\\321" (5 characters including a null terminator) or
// "0xfff" (this representation style is used for some characters if
// CHAR_BIT > 8).
#define CHAR_REPR_SIZE (CHAR_BIT / 4 + 3)


// A bytestring represents a sequence of (8-bit) bytes.
//
// `len` is the number of bytes contained in `data`.  Even if CHAR_BIT > 8,
// byte values are always in the range [0, 255].
//
typedef struct {
    size_t len;
    unsigned char *data;
} bytestring;


// `base64_digits` contains the digits used in base64, in order.
static const char base64_digits[64] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char padding = '=';


// Return the program name.
static const char *program_name(const char *const argv[],
                                const char *default_name)
{
    if (argv[0] != NULL && argv[0][0] != '\0') {
        return argv[0];
    }
    return default_name;
}

// Print a usage message to stderr.
static void print_usage(const char *const argv[])
{
    const char *name = program_name(argv, "s1c1.exe");
    fprintf(stderr, "Usage: %s hexstring\n", name);
}

// Write a human-readable version of a character to a string.
static void char_repr(char str[static CHAR_REPR_SIZE], char c)
{
    if (isgraph(c)) {
        str[0] = c;
        str[1] = '\0';
        return;
    }

    // If a character is widely known by its escape sequence, write the escape
    // sequence.  We can safely use strcpy() here because CHAR_REPR_SIZE >= 5.
    switch (c) {
    case '\n':
        strcpy(str, "\\n");
        return;
    case '\r':
        strcpy(str, "\\r");
        return;
    case '\t':
        strcpy(str, "\\t");
        return;
    case ' ':
        strcpy(str, "' '");
        return;
    }

    if ((unsigned char)c <= 255) {
        snprintf(str, CHAR_REPR_SIZE, "\\%o", (unsigned char)c);
        return;
    }
    snprintf(str, CHAR_REPR_SIZE, "0x%x", (unsigned char)c);
}

// Return the bytestring representation of a hex string or NULL on error.
//
// The bytestring should be freed by the caller.
//
static bytestring *bytes_from_hex(const char *hex)
{
    size_t num_digits = strlen(hex);
    for (size_t i = 0; i < num_digits; i++) {
        if (!isxdigit(hex[i])) {
            char readable[CHAR_REPR_SIZE];
            char_repr(readable, hex[i]);
            fprintf(stderr, "Invalid hex digit: %s\n", readable);
            return NULL;
        }
    }

    if (num_digits % 2 == 1) {
        fputs("Error: odd number of hex digits\n", stderr);
        return NULL;
    }

    bytestring *bytes;
    size_t num_bytes = num_digits / 2;
    if (num_bytes > SIZE_MAX - sizeof *bytes) {
        fputs("Error: can't allocate memory for bytestring\n", stderr);
        return NULL;
    }
    bytes = malloc(sizeof *bytes + num_bytes);
    if (bytes == NULL) {
        fputs("Error: failed to allocate memory for bytestring\n", stderr);
        return NULL;
    }
    bytes->len = num_bytes;
    bytes->data = (unsigned char *)(bytes + 1);

    for (size_t i = 0; i < bytes->len; i++) {
        // Use an unsigned short instead of an unsigned char because the format
        // string length modifier 'hh' is unknown to some systems.
        unsigned short value;
        // This should never fail since we've already validated the hex string.
        bool success = sscanf(&hex[2 * i], "%2hx", &value) != EOF;
        assert(success);
        bytes->data[i] = (unsigned char)value;
    }
    return bytes;
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
    bytestring *bytes = bytes_from_hex(hex);
    if (bytes == NULL) {
        return false;
    }

    print_as_base64(bytes);
    free(bytes);
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
