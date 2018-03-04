#include <stdint.h>
#include "assert.h"
#include "vector.h"

void new_vector(Vector *vector, uint32_t size, float arr[size])
{
	vector->size = size;
	vector->vector = arr;
}

float dot_product(Vector u, Vector v)
{
	uint32_t i;
	float dp = 0.0;

	if (u.size != v.size) return NaN;
	
	for (i = 0; i < u.size; i++)
		dp += u.vector[i] * v.vector[i];

	return dp;
}
