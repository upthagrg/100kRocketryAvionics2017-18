#include <stdint.h>
#include "assert.h"
#include "vector.h"
#include "matrix.h"

void new_matrix(Matrix *matrix, uint32_t rows, uint32_t cols,
                float arr[rows][cols])
{
	matrix->rows = rows;
	matrix->cols = cols;
	matrix->matrix = arr;
}

void identity_matrix(Matrix *matrix)
{
	uint32_t i, j;

	for (i = 0; i < matrix->rows; i++)
		for (j = 0; j < matrix->cols; j++)
			if (i == j) matrix->matrix[i][j] = 1;
			else matrix->matrix[i][j] = 0;
}

int8_t row_vector(Matrix matrix, uint32_t row, Vector *out)
{
	uint32_t i;

	assert(matrix.cols == out->size);

	for (i = 0; i < matrix.cols; i++)
		out->vector[i] = matrix.matrix[row][i];

	return 0;
}

int8_t column_vector(Matrix matrix, uint32_t column, Vector *out)
{
	uint32_t i;

	assert(matrix.rows == out->size);

	for (i = 0; i < matrix.rows; i++)
		out->vector[i] = matrix.matrix[i][column];

	return 0;
}

int8_t matrix_scale(Matrix in, float factor, Matrix *out)
{
	uint32_t i, j;

	assert(in.rows == out->rows);
	assert(in.cols == out->cols);
	
	for (i = 0; i < in.rows; i++)
		for (j = 0; j < in.cols; j++)
			out->matrix[i][j] = in.matrix[i][j] * factor;
	
	return 0;
}

int8_t matrix_transpose(Matrix in, Matrix *out)
{	
	uint32_t i, j;

	assert(in.rows == out->cols);
	assert(in.cols == out->rows);
	
	for (i = 0; i < in.rows; i++)
		for (j = 0; j < in.cols; j++)
			out->matrix[j][i] = in.matrix[i][j];

	return 0;
}

int8_t matrix_add(Matrix a, Matrix b, Matrix *out)
{
	uint32_t i, j;

	assert(a.rows == b.rows && a.rows == out->rows);
	assert(a.cols == b.cols && a.cols == out->cols);

	for (i = 0; i < a.rows; i++)
		for (j = 0; j < a.cols; j++)
			out->matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];

	return 0;
}

int8_t matrix_multiply(Matrix a, Matrix b, Matrix *out)
{
	uint32_t i, j;
	float r[a.rows];
	float c[b.cols];
	Vector u, v;
	float dp;
	
	assert(a.cols == b.rows);
	assert(a.rows == out->rows);
	assert(b.cols == out->cols);

	new_vector(&u, a.rows, r);
	new_vector(&v, b.rows, c);
	
	for (i = 0; i < a.cols; i++) {
		row_vector(a, i, &u);
		for (j = 0; j < a.cols; j++) {
			column_vector(b, j, &v);
			dp = dot_product(u, v);
			assert(dp != NaN);
			out->matrix[i][j] = dp;
		}
	}

	return 0;
}

int8_t matrix_inverse(Matrix matrix, Matrix *out)
{
	/* TODO: Implement */
	return 0;
}
