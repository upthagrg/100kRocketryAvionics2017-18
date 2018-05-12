#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <signal.h>
#include "debug.h"

void die(const char *message)
{
#ifdef DEBUG
	int size = 10000;
	void *buffer[size];
#endif
	fputs(message, stderr);
	fputc('\n', stderr);
#ifdef DEBUG
	size = backtrace(buffer, size);
	backtrace_symbols_fd(buffer, size, 2);
#endif
	exit(1);
}

void segv_handler(int signal)
{
#ifdef DEBUG
	int size = 10000;
	void *buffer[size];
#endif
	if (signal != SIGSEGV) return;
	fputs("Segmentation fault", stderr);
	fputc('\n', stderr);
#ifdef DEBUG
	size = backtrace(buffer, size);
	backtrace_symbols_fd(buffer, size, 2);
#endif
	abort();
}

void set_base_pointer(void)
{
	__asm__ __volatile__ ("movq %%rbp,%%rax" : "=a"(EBP) );
}

void *get_base_pointer(void)
{
	return EBP;
}

void *get_stack_pointer(void)
{
	void *p;
	__asm__ __volatile__ ("movq %%rsp,%%rax" : "=a"(p) );
	return p;
}

void set_max_memory(void)
{
	mem_max = MAX(mem_max, (unsigned long)get_base_pointer() - (unsigned long)get_stack_pointer());
}

unsigned long get_max_memory(void)
{
	return mem_max;
}
