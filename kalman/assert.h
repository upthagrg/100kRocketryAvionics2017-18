#ifndef ASSERT_H
#define ASSERT_H

#define ERROR -1
#define NaN (0.0f/0.0f)

#ifdef DEBUG
#include <assert.h>
#else
#define assert(EX) if (!(EX)) return ERROR
#endif


#endif
