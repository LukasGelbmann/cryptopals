#ifndef BYTES_H
#define BYTES_H

// This header defines the type `bytestring` and other tools related to bytes.

#include <stdbool.h>
#include <stddef.h>


// A bytestring represents a sequence of (8-bit) bytes.
//
// `len` is the number of bytes contained in `data`.  Even if CHAR_BIT > 8,
// byte values are always in the range [0, 255].
//
typedef struct {
    size_t len;
    unsigned char *data;
} bytestring;


// The respective numeric values of the first and the last printable ASCII
// character.
extern const unsigned char first_printable_ascii;
extern const unsigned char last_printable_ascii;


// Free the memory space used by a bytestring.
//
// If `bytes` is NULL, no operation is performed.
//
void free_bytestring(bytestring *bytes);

// Return the bytestring representation of a hex string or NULL on error.
//
// The bytestring should be freed by the caller.
//
bytestring *decode_hex(const char *hex);

// Print a bytestring in its hexadecimal representation.
void print_as_hex(const bytestring *bytes);

// Return true if the given byte stands for a printable character in ASCII.
bool is_printable_ascii(unsigned char byte);

// Print a bytestring, decoded as ASCII, and return true on success.
//
// This function returns true if each byte stands for a character that we can
// output regardless of the machine's encoding (this is the case for all
// printable ASCII characters and some others).
//
bool print_as_ascii(const bytestring *bytes);

#endif
