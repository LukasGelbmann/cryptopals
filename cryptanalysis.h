#ifndef CRYPTANALYSIS_H
#define CRYPTANALYSIS_H

// This header defines tools related to cryptanalysis.


// Return the natural logarithm of an ASCII character's relative frequency.
//
// A character's relative frequency is measured among printable ASCII
// characters in English-language text.  If the ASCII character represented by
// `byte` is non-printable, return -HUGE_VAL.
//
double english_log_frequency(unsigned char byte);

#endif
