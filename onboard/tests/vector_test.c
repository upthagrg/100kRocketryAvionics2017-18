#include "../assert.h"
#include "../vector.h"
#include "vector_test.h"

int main(void)
{
	vector_init_test();
	vector_dot_product_test();
	return 0;
}

int8_t vector_init_test(void)
{
	Vector u;
	float arr[10];
	uint32_t i = 0; /* Iteration variable */
	for(i = 0; i < 10; i++){
		arr[i] = (float) i;
	}
	new_vector(&u, 10, arr);

	assert(u.size == 10);
	for(i = 0; i < u.size; i++){
		assert(u.vector[i] == i);
	}
	return 0;
}

int8_t vector_dot_product_test(void)
{
	Vector a, b, c;
	uint32_t i = 0; /* Iteration variable */
	float arra[10], arrb[10], arrc[9];

	/* Build arrays */
	for(i = 0; i < 9; i++){
		arra[i] = (float) i;
		arrb[i] = 0;
		arrc[i] = (float) (i + i);
	}
	arra[9] = 9;
	arrb[9] = 0;

	/* Initialize vectors */
	new_vector(&a, 10, arra);
	new_vector(&b, 10, arrb);
	new_vector(&c,  9, arrc);

	/* Run tests */
	assert(dot_product(a, a) == 285.0);
	assert(dot_product(a, b) == 0.0);
	assert(dot_product(a, c) == NaN); /* Different sized arrays */
	return 0;
}
