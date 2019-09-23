#!/bin/sh --

REAL_PATH="$(readlink -f -- "$0" 2>/dev/null)" || REAL_PATH="$0"
TEST_PATH="$(dirname -- "$REAL_PATH")" || exit $?

cd -- "$TEST_PATH" || exit $?
. ./testing.sh


main() {(
    cd ../bin || return $?
    set_program_name s1c2

    test_output '"" ""' ''
    test_output '340056 de0000' ea0056

    input_a='1c0111001f010100061a024b53535009181c'
    input_b='686974207468652062756c6c277320657965'
    output='746865206b696420646f6e277420706c6179'
    test_output "$input_a $input_b" "$output"

    test_error '01' "should error with one argument"
    test_error '02 03 00' "should error with three arguments"
    test_error 'FF FG' "should error on invalid hex digit"
    test_error 'abc def' "should error on odd number of hex digits"
    test_error '01 2300' "should error if input B is longer"
    test_error '00 ""' "should error if input B is shorter"

    print_summary
    passed_all
)}

main
