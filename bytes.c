#include "bytes.h"

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


const unsigned char first_printable_ascii = 32;
const unsigned char last_printable_ascii = 126;

static const char printable_ascii[95] =
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdef"
    "ghijklmnopqrstuvwxyz{|}~";


void free_bytestring(bytestring *bytes)
{
    free(bytes);
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

bytestring *decode_hex(const char *hex)
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

void print_as_hex(const bytestring *bytes)
{
    for (size_t i = 0; i < bytes->len; i++) {
        printf("%02x", bytes->data[i]);
    }
    printf("\n");
}

bool is_printable_ascii(unsigned char byte)
{
    return first_printable_ascii <= byte && byte <= last_printable_ascii;
}

// Decode a byte as ASCII, print it, and return true on success.
//
// This function returns true if `byte` stands for a character that we can
// portably output (this is the case for all printable ASCII characters and
// some others).
//
static bool print_byte_as_ascii(unsigned char byte)
{
    if (is_printable_ascii(byte)) {
        putchar(printable_ascii[byte - first_printable_ascii]);
        return true;
    }

    switch (byte) {
    case 0x07:
        putchar('\a');
        return true;
    case 0x08:
        putchar('\b');
        return true;
    case 0x09:
        putchar('\t');
        return true;
    case 0x0a:
        putchar('\n');
        return true;
    case 0x0b:
        putchar('\v');
        return true;
    case 0x0c:
        putchar('\f');
        return true;
    case 0x0d:
        putchar('\r');
        return true;
    }

    return false;
}

bool print_as_ascii(const bytestring *bytes)
{
    bool success = true;
    for (size_t i = 0; i < bytes->len; i++) {
        if (!print_byte_as_ascii(bytes->data[i])) {
            success = false;
        }
    }
    printf("\n");
    return success;
}
