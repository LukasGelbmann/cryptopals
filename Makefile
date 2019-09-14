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

headers = bytestring.h commandline.h
objects := $(headers:%.h=obj/%.o)


.PHONY: all clean
.SECONDARY:

all:
	@echo "Please specify a target, e.g.:"
	@echo "    make bin/s1c1"
	@echo "    make clean"

bin/%: obj/%.o $(objects) | bin
	$(CC) $(CFLAGS) -o $@ $^

$(objects): obj/%.o: %.c %.h | obj
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: %.c $(headers) | obj
	$(CC) $(CFLAGS) -c -o $@ $<

bin obj:
	mkdir $@

clean:
	rm -f bin/* obj/*
