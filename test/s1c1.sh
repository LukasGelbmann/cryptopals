#!/bin/sh --

REAL_PATH="$(readlink -f -- "$0" 2>/dev/null)" || REAL_PATH="$0"
TEST_PATH="$(dirname -- "$REAL_PATH")" || exit $?

cd -- "$TEST_PATH" || exit $?
. ./testing.sh


main() {(
    cd ../bin || return $?
    set_program_name s1c1

    test_output '""' ''
    test_output ab qw==
    test_output 000d3eff50 AA0+/1A=

    hex='49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d'
    base64='SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t'
    test_output "$hex" "$base64"

    test_error '' "should error with no arguments"
    test_error '01 02' "should error with two arguments"
    test_error fffg "should error on invalid hex digit"
    test_error 0a0 "should error on odd number of hex digits"

    print_summary
    passed_all
)}

main
