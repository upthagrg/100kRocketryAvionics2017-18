#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

typedef struct Vector {
	uint32_t size;
	float *vector;
} Vector;

/*
  Initializes the vector `vector' of size `size' stored in `arr'
 */
void new_vector(Vector *vector, uint32_t size, float arr[size]);

/*
  Returns the dot product of vectors `u' and `v', or NaN if not possible
 */
float dot_product(Vector u, Vector v);

#endif
