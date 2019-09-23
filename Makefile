CC = gcc

CFLAGS = -std=c99 -O2 -flto -pedantic -Wall
ifeq ($(debug), 1)
    # -Wno-unknown-warning-option is given so that a user can specify
    # CC=clang and debug=1 as command-line arguments.
    CFLAGS += \
        -g -save-temps=obj -Wno-unknown-warning-option -Wno-type-limits \
        -Werror -Wextra -Wbad-function-cast -Wcast-align=strict -Wcast-qual \
        -Wconversion -Wdouble-promotion -Wduplicated-branches \
        -Wduplicated-cond -Wfloat-equal -Wformat=2 -Winit-self -Winline \
        -Winvalid-pch -Wjump-misses-init -Wlogical-not-parentheses \
        -Wlogical-op -Wmissing-declarations -Wmissing-format-attribute \
        -Wmissing-include-dirs -Wmissing-prototypes -Wnested-externs \
        -Wnull-dereference -Wold-style-definition -Wredundant-decls -Wshadow \
        -Wstrict-overflow=5 -Wstrict-prototypes -Wundef -Wwrite-strings
endif

programs := $(sort $(wildcard s?c?.c))
bins := $(programs:%.c=bin/%)
objs := $(programs:%.c=obj/%.o)

module_headers = $(sort $(wildcard *.h))
module_objs := $(module_headers:%.h=obj/%.o)

test_programs := $(sort $(wildcard test/test_*.c))
test_bins := $(test_programs:test/%.c=test/bin/%)
test_objs := $(test_programs:test/%.c=test/obj/%.o)
unit_tests := $(test_programs:test/%.c=%)

testing_module_headers = $(sort $(wildcard test/*.h))
testing_module_objs := $(testing_module_headers:test/%.h=test/obj/%.o)

test_scripts := $(sort $(wildcard test/s?c?.sh))
sh_tests := $(test_scripts:test/%.sh=test_%)


.PHONY: all test clean $(unit_tests) $(sh_tests)
.SECONDARY:

all: $(bins)

test: $(unit_tests) $(sh_tests)

$(unit_tests): %: test/bin/%
	@$< || true

$(sh_tests): test_%: bin/%
	@sh test/$*.sh || true

$(bins): bin/%: obj/%.o $(module_objs) | bin

$(test_bins): test/bin/test_%: test/obj/test_%.o obj/%.o $(testing_module_objs)
$(test_bins): | test/bin

# The mv command is for compatibility with Windows.
$(bins) $(test_bins):
	$(CC) $(CFLAGS) -o $@ $^
	@[ ! -e $@.exe ] || mv $@.exe $@

$(objs) $(module_objs): obj/%.o: %.c | obj
$(objs): $(module_headers)
$(module_objs): obj/%.o: %.h

$(test_objs) $(testing_module_objs): test/obj/%.o: test/%.c | test/obj
$(test_objs): test/obj/test_%.o: %.h $(testing_module_headers)
$(testing_module_objs): test/obj/%.o: test/%.h

$(objs) $(module_objs) $(test_objs) $(testing_module_objs):
	$(CC) $(CFLAGS) -c -o $@ $<

bin obj test/bin test/obj:
	mkdir $@

clean:
	rm -f bin/* obj/* test/bin/* test/obj/*
