#ifndef UNITTEST_H
#define UNITTEST_H

// This header defines tools related to unit testing.

#include <stdbool.h>


typedef struct {
    const char *name;
    int passed;
    int failed;
} tests_summary;


// Print how many tests we passed and how many we failed.
void print_summary(const tests_summary *summary);

// Increase the counter of passed tests by one.
void count_passed_test(tests_summary *summary);

// Record a test that checks whether a condition is true.
void test_that(bool passed, tests_summary *summary, const char *message);

// Record a test that checks whether two strings are equal.
void test_str_equals(const char *result, const char *expected,
                     const char *test_name, tests_summary *summary);

// Redirect stderr to /dev/null, return a file descriptor for the old stderr.
int discard_stderr(void);

// Redirect stderr to the file associated with the given file descriptor.
void restore_stderr(int old_stderr_fd);

#endif
