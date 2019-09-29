#!/bin/sh --

REAL_PATH="$(readlink -f -- "$0" 2>/dev/null)" || REAL_PATH="$0"
TEST_PATH="$(dirname -- "$REAL_PATH")" || exit $?

cd -- "$TEST_PATH" || exit $?
. ./testing.sh


main() {(
    cd ../bin || return $?
    set_program_name s1c3

    test_output '""' ''

    xored='9bbcaebfa8fabbfabcbbb6b6faa9afb9b2fabba9faaeb2b3a9f6fa93faa9b2bbb6b6faaeb2b3b4b1fab4b5aeb2b3b4bdfab5bcfaaeafb7b8b6b3b4bdfabeb5adb4a9aebbb3a8a9fb'
    text='After a fall such as this, I shall think nothing of tumbling downstairs!'
    test_output "$xored" "$text"

    ciphertext='1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736'
    plaintext="Cooking MC's like a pound of bacon"
    test_output "$ciphertext" "$plaintext"

    test_error '' "should error with no arguments"
    test_error '01 02' "should error with two arguments"
    test_error ..ad "should error on invalid hex digit"
    test_error 789 "should error on odd number of hex digits"

    # There exists no key that would make both bytes of this ciphertext decrypt
    # to ASCII characters.
    test_error 0080 "should error on non-ASCII plaintext"

    print_summary
    passed_all
)}

main
