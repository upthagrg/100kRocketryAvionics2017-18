CC			= gcc
CCFLAGS		= -ansi -pedantic -Werror=trigraphs -Wall -Wextra\
			  -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align\
			  -Wstrict-prototypes -Wmissing-prototypes -Wconversion\
			  -Wno-vla #-Wtraditional
DEBUGFLAGS	= -g -DDEBUG
TARGET		= kalman
HEADERS		= kalman.h matrix.h vector.h hwint.h debug.h
SRCS		= kalman.c matrix.c vector.c main.c hwint.c
DEBUGSRCS	= kalman.c matrix.c vector.c main.c hwsim.c debug.c conversion.c
TESTS		= tests/*

.PHONY: clean all tests

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CCFLAGS) $(SRCS) -o $@

asm: $(SRCS) $(HEADERS)
	$(CC) -S $(CCFLAGS) $(SRCS)

debug: $(SRCS) $(HEADERS)
	$(CC) $(DEBUGFLAGS) $(DEBUGSRCS) -o $@

tests: vector_test matrix_test

vector_test: $(SRCS) $(HEADERS) $(TESTS)
	$(CC) $(CCFLAGS) vector.c tests/vector_test.c -o $@

matrix_test: $(SRCS) $(HEADERS) $(TESTS)
	$(CC) $(CCFLAGS) vector.c matrix.c tests/matrix_test.c -o $@

clean:
	rm -f $(TARGET) debug matrix_test vector_test
