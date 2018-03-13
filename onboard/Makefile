CC			= gcc
CCFLAGS		= -ansi -pedantic -Werror=trigraphs -Wall -Wextra\
			  -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align\
			  -Wstrict-prototypes -Wmissing-prototypes -Wconversion\
			  -Wno-vla #-Wtraditional
DEBUGFLAGS	= -g -DDEBUG
TARGET		= 
HEADERS		= kalman.h matrix.h vector.h
SRCS		= kalman.c matrix.c vector.c
TESTS		= tests/*

.PHONY: clean all debug tests

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CCFLAGS) $(SRCS) -o $@

debug: $(SRCS) $(HEADERS)
	$(CC) $(DEBUGFLAGS) $(CCFLAGS) $(SRCS) -o $(TARGET)

tests: vector_test matrix_test

vector_test: $(SRCS) $(HEADERS) $(TESTS)
	$(CC) $(CCFLAGS) vector.c tests/vector_test.c -o $@

matrix_test: $(SRCS) $(HEADERS) $(TESTS)
	$(CC) $(CCFLAGS) vector.c matrix.c tests/matrix_test.c -o $@

clean:
	rm -f $(TARGET) matrix_test vector_test
