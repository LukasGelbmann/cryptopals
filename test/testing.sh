# Tools to enable integration testing.

_NEWLINE='
'
_CARRIAGE_RETURN=$(printf '\r')
_RED='\033[1;31m'
_GREEN='\033[1;32m'
_END_COLOR='\033[0m'

_program_name='dummy_program_name'
_passed=0
_failed=0


# Globally set the program name.
set_program_name() {
    _program_name="$1"
}

# Print how many tests we passed and how many we failed.
print_summary() {(
    if [ -t 1 ]; then
        if passed_all; then
            color="$_GREEN"
        else
            color="$_RED"
        fi
        end="$_END_COLOR"
    else
        color=''
        end=''
    fi

    if [ "$_passed" -ne 1 ]; then
        ending='s'
    else
        ending=''
    fi

    message="Passed ${_passed} test${ending} and failed ${_failed}."
    echo "${color}${_program_name}: ${message}${end}"
)}

# Return 0 (success) if we passed all tests.
passed_all() {
    [ "$_failed" -eq 0 ]
}

# Run a test that checks whether the output is as expected.
test_output() {
    _run_test _check_output "$@"
}

# Run a test that expects an error.
test_error() {
    _run_test _check_error "$@"
}

# Run a test and update the counters of passed and failed tests.
_run_test() {
    if "$@"; then
        _passed=$((_passed + 1))
    else
        _failed=$((_failed + 1))
    fi
}

# Run a command and return 0 (success) if the output is as expected.
_check_output() {(
    command="./$_program_name $1"
    expected="$2"

    output_dot=$(sh -c "$command"; ret=$?; echo .; exit "$ret")
    ret=$?
    if [ "$ret" != 0 ]; then
        _print_failure "got exit code $ret" "$command"
        return 1
    fi

    # Under Windows, a carriage return may have been added to the output.
    output=${output_dot%.}
    exp_lf="${expected}${_NEWLINE}"
    exp_crlf="${expected}${_CARRIAGE_RETURN}${_NEWLINE}"
    if [ "$output" != "$exp_lf" ] && [ "$output" != "$exp_crlf" ]; then
        if [ "$output" = "$expected" ]; then
            _print_failure "missing newline" "$command"
        else
            stripped=${output%$_NEWLINE}
            stripped=${stripped%$_CARRIAGE_RETURN}
            _print_failure "expected '$expected', got '$stripped'" "$command"
        fi
        return 1
    fi

    return 0
)}

# Run a command and return 0 (success) if it returns exit code 1 and no output.
_check_error() {(
    command="./$_program_name $1"
    message="$2"

    output_dot=$(sh -c "$command" 2>/dev/null; ret=$?; echo .; exit "$ret")
    ret=$?
    if [ "$ret" != 1 ]; then
        _print_failure "expected exit code 1, got $ret" "$message"
        return 1
    fi

    output=${output_dot%.}
    if [ -n "$output" ]; then
        _print_failure "got non-empty output" "$message"
        return 1
    fi

    return 0
)}

# Print a message describing a failed test.
_print_failure() {(
    diagnostic="$1"
    message="$2"
    echo "[$_program_name] Failed test ($diagnostic): $message"
)}
