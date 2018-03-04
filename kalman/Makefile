CC			= gcc
CCFLAGS		= -ansi -pedantic -Werror=trigraphs -Wall -Wextra\
			  -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align\
			  -Wstrict-prototypes -Wmissing-prototypes -Wconversion\
			  -Wno-vla #-Wtraditional
DEBUGFLAGS	= -g -DDEBUG
TARGET		= kalman
HEADERS		= $(TARGET).h matrix.h vector.h
SRCS		= $(TARGET).c matrix.c vector.c main.c

.PHONY: clean all debug

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CCFLAGS) $(SRCS) -o $@

debug: $(SRCS) $(HEADERS)
	$(CC) $(DEBUGFLAGS) $(CCFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
