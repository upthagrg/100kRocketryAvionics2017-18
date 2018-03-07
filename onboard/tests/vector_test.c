#include "../assert.h"
#include "../vector.h"
#include "vector_test.h"

int main(void)
{
	vector_init_test();
	vector_dot_product_test();
	return 0;
}

void vector_init_test(void)
{
	Vector u;
	float arr[10];
	int i = 0; // Iteration variable
	for(i; i < 10; i++)
		arr[i] = i;
	
	new_vector(&u, 10, arr);
	
	assert(u.size == 10);
	for(i = 0; i < u.size; i++)
		assert(u.vector[i] == i);
	
	return;
}

void vector_dot_product_test(void)
{
	Vector a, b, c;
	int i = 0; // Iteration variable
	
	// Build arrays
	float arra[10], arrb[10], arrc[9];
	for(i = 0; i < 9; i++){
		arra[i] = i;
		arrb[i] = 0;
		arrc[i] = i + i;
	}
	arra[9] = 9;
	arrb[9] = 81;
	
	// Initialize vectors
	new_vector(&a, 10, arra);
	new_vector(&b, 10, arrb);
	new_vector(&c,  9, arrc);
	
	// Run tests
	assert(dot_product(a, a) == 285);
	assert(dot_product(a, b) == 0);
	assert(dot_product(a, c) == NaN); // Different sized arrays
	return;
}
