#include "cryptanalysis.h"

#include "bytes.h"

#include <math.h>


// Relative frequency of each printable ASCII character among printable ASCII
// characters in English-language text.
//
// The following values were computed on the basis of the full contents of the
// English Wikipedia in February 2018.  The contents of the English Wikipedia
// were obtained from Wikipedia and compiled by Rudolf Rosa and published as
// "Plaintext Wikipedia dump 2018" (https://hdl.handle.net/11234/1-2735).
//
static const double english_frequencies[] = {
    /*   */ 0.160354013, /* ! */ 3.628767e-5, /* " */ 0.003794262,
    /* # */ 2.250957e-5, /* $ */ 8.011782e-5, /* % */ 1.889992e-4,
    /* & */ 7.549149e-5, /* ' */ 0.001358827, /* ( */ 0.001383028,
    /* ) */ 0.001385123, /* * */ 7.909008e-6, /* + */ 1.488917e-5,
    /* , */ 0.009838256, /* - */ 0.001919591, /* . */ 0.008670874,
    /* / */ 1.610075e-4, /* 0 */ 0.003381571, /* 1 */ 0.004286811,
    /* 2 */ 0.002511673, /* 3 */ 0.001096818, /* 4 */ 0.001055557,
    /* 5 */ 0.001132889, /* 6 */ 0.001042755, /* 7 */ 0.001027034,
    /* 8 */ 0.001295095, /* 9 */ 0.002398044, /* : */ 4.000888e-4,
    /* ; */ 2.719602e-4, /* < */ 1.028906e-6, /* = */ 3.016027e-5,
    /* > */ 1.621360e-6, /* ? */ 1.806388e-5, /* @ */ 1.035566e-6,
    /* A */ 0.003426730, /* B */ 0.002203506, /* C */ 0.003300183,
    /* D */ 0.001646432, /* E */ 0.001223534, /* F */ 0.001508965,
    /* G */ 0.001346110, /* H */ 0.001946920, /* I */ 0.002348129,
    /* J */ 0.001085204, /* K */ 8.306436e-4, /* L */ 0.001499211,
    /* M */ 0.002504257, /* N */ 0.001360306, /* O */ 0.001042820,
    /* P */ 0.001977080, /* Q */ 1.226392e-4, /* R */ 0.001636191,
    /* S */ 0.003724892, /* T */ 0.003695593, /* U */ 7.808417e-4,
    /* V */ 6.144631e-4, /* W */ 0.001290482, /* X */ 8.811673e-5,
    /* Y */ 3.074767e-4, /* Z */ 1.553328e-4, /* [ */ 3.865882e-5,
    /* \ */ 1.096751e-6, /* ] */ 3.916902e-5, /* ^ */ 4.667538e-7,
    /* _ */ 3.537040e-5, /* ` */ 1.285800e-6, /* a */ 0.066947491,
    /* b */ 0.010497989, /* c */ 0.023543998, /* d */ 0.029713818,
    /* e */ 0.092730974, /* f */ 0.016253178, /* g */ 0.014321344,
    /* h */ 0.035449270, /* i */ 0.057854352, /* j */ 8.219515e-4,
    /* k */ 0.004656964, /* l */ 0.031884137, /* m */ 0.018140895,
    /* n */ 0.057931719, /* o */ 0.056304163, /* p */ 0.014312417,
    /* q */ 6.632917e-4, /* r */ 0.050952658, /* s */ 0.048449399,
    /* t */ 0.063603945, /* u */ 0.020153464, /* v */ 0.007672665,
    /* w */ 0.011905618, /* x */ 0.001407822, /* y */ 0.011753092,
    /* z */ 0.001026721, /* { */ 1.370876e-6, /* | */ 1.649280e-5,
    /* } */ 1.440219e-6, /* ~ */ 1.858358e-6,
};


double english_log_frequency(unsigned char byte)
{
    if (!is_printable_ascii(byte)) {
        return -HUGE_VAL;
    }
    double frequency = english_frequencies[byte - first_printable_ascii];
    return log(frequency);
}
