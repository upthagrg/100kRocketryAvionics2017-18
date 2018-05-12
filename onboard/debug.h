#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "conversion.h"

#define MAX(x, y) (x > y ? x : y)
#define printsize(v) printf(#v": %lu\n", sizeof(v))

void *EBP;
unsigned long mem_max;
FILE *debug_input;
struct telem_data _hwsim_td;

void die(const char *);

void segv_handler(int);

void set_base_pointer(void);

void *get_base_pointer(void);

void *get_stack_pointer(void);

void set_max_memory(void);

unsigned long get_max_memory(void);

#endif
