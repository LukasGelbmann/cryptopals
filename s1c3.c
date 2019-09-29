// Break the single-byte XOR cipher.
//
// This program takes as an argument a hex-encoded ciphertext, which has been
// encrypted by XORing the plaintext against a single byte.  The plaintext is
// assumed to be in English and to consist of printable ASCII characters.  The
// program breaks the cipher through frequency analysis and outputs the
// plaintext message.

#include "bytes.h"
#include "commandline.h"
#include "cryptanalysis.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Return the log-likelihood that a key was used to encrypt a ciphertext.
//
// This function assumes that the plaintext is in English and consists of
// printable ASCII characters.  If no key would make all characters of the
// plaintext printable ASCII, return -HUGE_VAL (which corresponds to a
// likelihood of 0).
//
static double log_likelihood(unsigned char key, const bytestring *ciphertext)
{
    double log_L = 0.0;
    for (size_t i = 0; i < ciphertext->len; i++) {
        unsigned char plain = ciphertext->data[i] ^ key;
        if (!is_printable_ascii(plain)) {
            return -HUGE_VAL;
        }
        log_L += english_log_frequency(plain);
    }
    return log_L;
}

// Return the key likeliest to have been used to encrypt a ciphertext.
//
// Find the likeliest key assuming that the plaintext is in English and
// consists of printable ASCII characters.  If no key would make all characters
// of the plaintext printable ASCII, return -1.
//
static int break_key(const bytestring *ciphertext)
{
    int likeliest_key = -1;
    double max_log_L = -HUGE_VAL;
    for (int key = 0; key < 256; key++) {
        double log_L = log_likelihood((unsigned char)key, ciphertext);
        if (log_L > max_log_L) {
            likeliest_key = key;
            max_log_L = log_L;
        }
    }
    return likeliest_key;
}

// Decrypt a bytestring in place by performing an XOR on each byte.
static void decrypt(const bytestring *bytes, unsigned char key)
{
    for (size_t i = 0; i < bytes->len; i++) {
        bytes->data[i] ^= key;
    }
}

// Try to find the decrypted string, print it, and return true on success.
static bool print_decrypted(const char *hex_ciphertext)
{
    bytestring *bytes = decode_hex(hex_ciphertext);
    if (bytes == NULL) {
        return false;
    }

    int key = break_key(bytes);
    if (key == -1) {
        fputs("Error: no key turns all bytes into printable ASCII\n", stderr);
    } else {
        decrypt(bytes, (unsigned char)key);
        // Since break_key() returned a valid key, all plaintext bytes should
        // be printable ASCII, which can be output by print_as_ascii().
        bool is_good_ascii = print_as_ascii(bytes);
        assert(is_good_ascii);
    }
    free_bytestring(bytes);
    return key != -1;
}

// Print the plaintext corresponding to the ciphertext given as an argument.
int main(int argc, const char *const argv[])
{
    if (argc != 2) {
        print_usage(argv, "s1c3", "hex_ciphertext");
        return EXIT_FAILURE;
    }
    if (!print_decrypted(argv[1])) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
