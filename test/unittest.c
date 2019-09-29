#include "unittest.h"

#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


static const char red[] = "\033[1;31m";
static const char green[] = "\033[1;32m";
static const char end_color[] = "\033[0m";


void print_summary(const tests_summary *summary)
{
    const char *color = "";
    const char *end = "";
    if (isatty(STDOUT_FILENO)) {
        color = summary->failed == 0 ? green : red;
        end = end_color;
    }

    const char *ending = summary->passed != 1 ? "s" : "";
    printf("%s%s: Passed %d ", color, summary->name, summary->passed);
    printf("test%s and failed %d.%s\n", ending, summary->failed, end);
}

void print_failure_context(const char *program_name, const char *test_name)
{
    printf("[%s] Failed test %s: ", program_name, test_name);
}

// Print a message describing a failed test.
static void print_failure(const tests_summary *summary, const char *message)
{
    printf("[%s] Failed test: %s\n", summary->name, message);
}

// Update the counters of passed and failed tests.
static void count_test(bool passed, tests_summary *summary)
{
    if (passed) {
        summary->passed++;
    } else {
        summary->failed++;
    }
}

void count_passed_test(tests_summary *summary)
{
    count_test(true, summary);
}

void run_test(bool (*test)(const char *program_name), tests_summary *summary)
{
    count_test(test(summary->name), summary);
}

void test_that(bool passed, tests_summary *summary, const char *message)
{
    if (!passed) {
        print_failure(summary, message);
    }
    count_test(passed, summary);
}

void test_str_equals(const char *result, const char *expected,
                     const char *test_name, tests_summary *summary)
{
    bool passed = strcmp(result, expected) == 0;
    if (!passed) {
        print_failure_context(summary->name, test_name);
        printf("expected \"%s\", got \"%s\"\n", expected, result);
    }
    count_test(passed, summary);
}

int discard_stderr(void)
{
    fflush(stderr);
    int old_stderr_fd = dup(STDERR_FILENO);
    int dev_null_fd = open("/dev/null", O_WRONLY);
    dup2(dev_null_fd, STDERR_FILENO);
    close(dev_null_fd);
    return old_stderr_fd;
}

void restore_stderr(int old_stderr_fd)
{
    fflush(stderr);
    dup2(old_stderr_fd, STDERR_FILENO);
    close(old_stderr_fd);
}
