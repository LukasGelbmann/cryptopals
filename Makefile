CC = gcc

CFLAGS = -std=c99 -O2 -pedantic -Wall
ifeq ($(debug), 1)
    # -Wno-unknown-warning-option is given so that a user can specify
    # CC=clang and debug=1 as command-line arguments.
    CFLAGS += \
        -g -save-temps -Wno-unknown-warning-option -Wno-type-limits -Werror \
        -Wextra -Wbad-function-cast -Wcast-align=strict -Wcast-qual \
        -Wconversion -Wdouble-promotion -Wduplicated-branches \
        -Wduplicated-cond -Wfloat-equal -Wformat=2 -Winit-self -Winline \
        -Winvalid-pch -Wjump-misses-init -Wlogical-not-parentheses \
        -Wlogical-op -Wmissing-declarations -Wmissing-format-attribute \
        -Wmissing-include-dirs -Wmissing-prototypes -Wnested-externs \
        -Wnull-dereference -Wold-style-definition -Wredundant-decls -Wshadow \
        -Wstrict-overflow=5 -Wstrict-prototypes -Wundef -Wwrite-strings
endif


.PHONY: all clean

all:
	@echo "Please specify a target, e.g.:"
	@echo "    make s1c1.exe"
	@echo "    make clean"

%.exe: %.c
	$(CC) $(CFLAGS) -o $@ $<

# LLVM IR bitcode files with the extension .bc are generated by clang.
clean:
	rm -f *.bc *.exe *.i *.o *.s
