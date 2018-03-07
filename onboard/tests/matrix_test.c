#include "../assert.h"
#include "../matrix.h"
#include "matrix_test.h"

int main(void)
{
	matrix_init_test();
	matrix_element_get_test();
	matrix_element_set_test();
	matrix_identity_test();
	matrix_row_vector_test();
	matrix_column_vector_test();
	matrix_row_add_test();
	matrix_row_scale_test();
	matrix_scale_test();
	matrix_transpose_test();
	matrix_add_test();
	matrix_multiply_test();
	matrix_inverse_test();
	return 0;
}

void matrix_init_test(void)
{
	Matrix m;
	int i, j; // Iteration variables
	float arr[10][10];
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arr[i][j] = i + j;
		
	new_matrix(&m, 10, 10, arr);
	
	// Check that all elements were correctly stored
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			assert(m.matrix[i][j] == i + j);
	
	return;
}

void matrix_element_get_test(void)
{
	Matrix m;
	int i, j; // Iteration variables
	float arr[10][10];
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arr[i][j] = i + j;
		
	new_matrix(&m, 10, 10, arr);
	
	// Check that all elements were correctly stored
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
				assert(matrix_element(m, i, j) == i + j);

	return;
}

void matrix_element_set_test(void)
{
	Matrix m;
	int i, j; // Iteration variables
	float arr[10][10];
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arr[i][j] = 0;
		
	new_matrix(&m, 10, 10, arr);
	matrix_element(m, 5, 6) = 11;
	
	// Check that all elements were correctly stored
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			if( i == 5 && j == 6)
				assert(matrix_element(m, i, j) == 11);
			else
				assert(matrix_element(m, i, j) == 0);
		}
	}
	return;
}

void matrix_identity_test(void)
{
	Matrix m1, m2;
	int i, j; // Iteration variables
	float arr1[10][10]; // Square matrix
	float arr2[5][3];  // Non-square matrix
	new_matrix(&m1, 10, 10, arr1);
	new_matrix(&m2, 10, 10, arr2);
	
	identity_matrix(&m1);
	identity_matrix(&m2);
	
	// Check that all elements were correctly stored in m1
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			if( i == j)
				assert(matrix_element(m1, i, j) == 1);
			else
				assert(matrix_element(m1, i, j) == 0);
		}
	}
	
	// Check that all elements were correctly stored in m2
	for(i = 0; i < 5; i++){
		for(j = 0; j < 3; j++){
			if( i == j)
				assert(matrix_element(m2, i, j) == 1);
			else
				assert(matrix_element(m2, i, j) == 0);
		}
	}
	
	return;
}

void matrix_row_vector_test(void)
{
	Matrix m1, m2;
	Vector v;
	int i, j; // Iteration variables
	float arrm1[10][10];
	float arrm2[5][3];
	float arrv[10];
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arrm1[i][j] = i + j;
	
	for(i = 0; i < 5; i++)
		for(j = 0; j < 3; j++)
			arrm2[i][j] = i + j;
	
	new_matrix(&m1, 10, 10, arrm1);
	new_matrix(&m2, 5, 3, arrm2);
	new_vector(&v, 10, arrv);
	
	assert(row_vector(&m2, 2, &v)  < 0);  // Different sizes
	assert(row_vector(&m1, 12, &v)  < 0); // Out of bounds
	assert(row_vector(&m1, 8, &v) == 0);  // Correct usage
	
	// Test output
	for(i = 0; i < 10; i++)
		assert(v.vector[i] == matrix_element(b, 8, i));
		
	
	return;
}

void matrix_column_vector_test(void)
{
	Matrix m1, m2;
	Vector v;
	int i, j; // Iteration variables
	float arrm1[10][10]; // Input
	float arrm2[5][3];   // Output
	float arrv[10];
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arrm1[i][j] = i + j;
	
	for(i = 0; i < 5; i++)
		for(j = 0; j < 3; j++)
			arrm2[i][j] = i + j;
	
	new_matrix(&m1, 10, 10, arrm1);
	new_matrix(&m2, 5, 3, arrm2);
	new_vector(&v, 10, arrv);
	
	assert(column_vector(&m2, 2, &v)  < 0);  // Different sizes
	assert(column_vector(&m1, 12, &v)  < 0); // Out of bounds
	assert(column_vector(&m1, 8, &v) == 0);  // Correct usage
	
	// Test output
	for(i = 0; i < 10; i++)
		assert(v.vector[i] == matrix_element(b, i, 8));
	
	return;
}

void matrix_row_add_test(void)
{
	Matrix a, b, c;
	int i, j;
	float arra[10][10]; // Input
	float arrc[5][5];   // Input/output
	
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arra[i][j] = i + j;
	
	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			arrc[i][j] = i + j;
		
	new_matrix(&a, 10, 10, arra);
	new_matrix(&b, 10, 10, arra);
	new_matrix(&c, 5, 5, arrc);
	
	// Test the operation
	assert(matrix_row_add(a, 5, &c, 3) < 0); // Input is larger than output
	assert(matrix_row_add(a, 12, &b, 3) < 0); // Out-of-bounds on input
	assert(matrix_row_add(a, 8, &b, 12) < 0); // Out-of-bounds on output
	assert(matrix_row_add(a, 8, &b, 8) == 0); // Correct operation, input and output are same size
	assert(matrix_row_add(c, 3, &a, 5) == 0); // Correct operation, input is smaller than output
	
	// Test the results
	for(i = 0; i < 10; i++) // Input and output are same size
		assert(matrix_element(b, 8, i) == 2 * (i + 8));
	
	for(i = 0; i < 10; i++){ // Input is smaller than output
		if(i < 5)
			assert(matrix_element(a, 5, i) == 2 * (i + 5));
		else
			assert(matrix_element(a, 5, i) == i + 5);
	}
	return;
}

void matrix_row_scale_test(void)
{
	Matrix m;
	int i, j; // Iteration variables
	float arr[10][10]; // Input/Output
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arr[i][j] = i + j;
		
	new_matrix(&m, 10, 10, arr);
	
	// Test function
	assert(matrix_row_scale(&m, 12, 2) < 0); // Out-of-bounds
	assert(matrix_row_scale(&m, 8, 2) == 0); // Correct operation
	
	// Test output
	for(i = 0; i < 10; i++)
		assert(matrix_element(a, 8, i) == 2 * (i + 8));
	return;
}

void matrix_scale_test(void)
{
	Matrix m;
	int i, j; // Iteration variables
	float arr[10][10]; // Input/Output
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arr[i][j] = i + j;
		
	new_matrix(&m, 10, 10, arr);
	
	// Test function
	assert(matrix_scale(&m, 2) == 0); // Correct operation
	
	// Test output
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			assert(matrix_element(a, i, j) == 2 * (i + j));
	return;
}

void matrix_transpose_test(void)
{
	Matrix a, b, c;
	int i, j;
	float arra[10][10]; // Input
	float arrb[10][10]; // Output
	float arrc[5][5];   // Error-case output
	
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			arra[i][j] = i + j;
		
	new_matrix(&a, 10, 10, arra);
	new_matrix(&b, 10, 10, arrb);
	new_matrix(&c, 5, 5, arrc);
	
	// Test function
	assert(matrix_transpose(a, &c) < 0); // Sizing mis-match
	assert(matrix_transpose(a, &b) == 0); // Correct operation
	
	// Test output
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			assert(matrix_element(a, i, j) == matrix_element(b, j, i));
	return;
}

void matrix_add_test(void)
{
	Matrix a, b, c, d;
	int i, j;
	float arra[10][10]; // Input
	float arrb[10][10]; // Input
	float arrc[5][5];   // Error-case output
	float arrd[10][10]; // Output
	
	for(i = 0; i < 10; i++){
		for(j = 0; j < 10; j++){
			arra[i][j] = i + j;
			arrb[i][j] = i - j;
		}
	}
	
	new_matrix(&a, 10, 10, arra);
	new_matrix(&b, 10, 10, arrb);
	new_matrix(&c, 5, 5, arrc);
	new_matrix(&d, 10, 10, arrd);
	
	// Test function
	assert(matrix_add(a, b, &c) < 0); // Output size is different
	assert(matrix_add(a, c, &b) < 0); // Inputs are different size
	assert(matrix_add(a, b, &d) == 0); // Successful operation
	
	// Test output
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			assert(matrix_element(d, i, j) == matrix_element(a, i, j) + matrix_element(b, i, j));
		
	return;
}

void matrix_multiply_test(void)
{
	Matrix a, b, c, d, e, f, g;
	float arra[5][3]; // Input
	float arrb[3][5]; // Input
	float arrc[5][5]; // Output
	float arrd[4][4]; // Bad size test
	float arre[3][3]; // Output
	float arrf[5][5]; // Output comparison
	float arrg[3][3]; // Output comparison
	int i, j;
	
	// Fill inputs
	// If you add more tests, you'll need to either add specific output arrays
	// Or a section that recomputes the dot-products for comparison
	// Because the arrays I've written here are small, it's simpler to check directly against the output
	// Than to fiddle with vectors for the dot product
	for(i = 0; i < 5; i++){
		for(j = 0; j < 3; j++){
			arra[i][j] = i + j;
			arrb[j][i] = i + j;
		}
	}
	
	// Fill the first output comparison
	// Formula determined by hand by computing a*b
	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			arrf[i][j] = 5 + 3*i + 3*(i+1)*j;
		
	// Fill the second output comparison
	// Formula also determined by hand-computation of b*a
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			arrg[i][j] = 30 + 10*i + j*(i*5 + 10);
	
	new_matrix(&a, 5, 3, arra);
	new_matrix(&b, 3, 5, arrb);
	new_matrix(&c, 5, 5, arrc);
	new_matrix(&d, 4, 4, arrd);
	new_matrix(&e, 3, 3, arre);
	new_matrix(&f, 5, 5, arrf);
	new_matrix(&g, 3, 3, arrg);
	
	// Test functions
	assert(matrix_multiply(a, b, &d) < 0);  // Output is improperly sized
	assert(matrix_multiply(a, b, &c) == 0); // Correct operation, fills matrix c
	assert(matrix_multiply(c, b, &c) < 0);  // Input is improperly sized
	assert(matrix_multiply(b, a, &e) == 0); // Correct operation
	
	// Test outputs
	for(i = 0; i < 5; i++)
		for(j = 0; j < 5; j++)
			assert((matrix_element(c, i, j) == matrix_element(f, i, j));
	
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			assert((matrix_element(e, i, j) == matrix_element(g, i, j));
	
	return;
}

void matrix_inverse_test(void)
{
	;
}
