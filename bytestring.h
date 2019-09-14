#ifndef BYTESTRING_H
#define BYTESTRING_H

// This header defines the type `bytestring` and related tools.

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

#endif
