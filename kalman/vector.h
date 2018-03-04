#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct Vector {
	uint32_t size;
	float *vector;
} Vector;

void new_vector(Vector *, uint32_t size, float [size]);

float dot_product(Vector, Vector);

#endif
