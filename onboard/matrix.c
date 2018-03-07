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

	for (i = 0; i < matrix->rows; i++) {
		for (j = 0; j < matrix->cols; j++) {
			if (i == j) matrix_element(*matrix, i, j) = 1;
			else matrix_element(*matrix, i, j) = 0;
		}
	}
}

int8_t row_vector(Matrix matrix, uint32_t row, Vector *out)
{
	uint32_t i;

	assert(matrix.cols == out->size);

	for (i = 0; i < matrix.cols; i++) {
		out->vector[i] = matrix_element(matrix, row, i);
	}

	return 0;
}

int8_t column_vector(Matrix matrix, uint32_t column, Vector *out)
{
	uint32_t i;

	assert(matrix.rows == out->size);

	for (i = 0; i < matrix.rows; i++) {
		out->vector[i] = matrix_element(matrix, i, column);
	}

	return 0;
}

int8_t matrix_row_add(Matrix in, uint32_t in_row, Matrix *out, uint32_t out_row)
{
	uint32_t i;

	assert(in_row < in.rows);
	assert(out_row < out->rows);
	assert(in.cols == out->cols);

	for (i = 0; i < in.cols; i++) {
		matrix_element(*out, out_row, i) += matrix_element(in, in_row, i);
	}

	return 0;
}

int8_t matrix_row_scale(Matrix *matrix, uint32_t row, float factor)
{
	uint32_t i;
	
	assert(row < matrix->rows);

	for (i = 0; i < matrix->cols; i++) {
		matrix_element(*matrix, row, i) *= factor;
	}

	return 0;
}

int8_t matrix_scale(Matrix matrix, float factor)
{
	uint32_t i, j;

	for (i = 0; i < matrix->rows; i++) {
		for (j = 0; j < matrix->cols; j++) {
			matrix_element(*matrix, i, j) *= factor;
		}
	}
	
	return 0;
}

int8_t matrix_transpose(Matrix in, Matrix *out)
{	
	uint32_t i, j;

	assert(in.rows == out->cols);
	assert(in.cols == out->rows);
	
	for (i = 0; i < in.rows; i++) {
		for (j = 0; j < in.cols; j++) {
			matrix_element(*out, j, i) = matrix_element(in, i, j);
		}
	}

	return 0;
}

int8_t matrix_add(Matrix a, Matrix b, Matrix *out)
{
	uint32_t i, j;

	assert(a.rows == b.rows);
	assert(a.rows == out->rows);
	assert(a.cols == b.cols);
	assert(a.cols == out->cols);

	for (i = 0; i < a.rows; i++) {
		for (j = 0; j < a.cols; j++) {
			matrix_element(*out, i, j) =
				matrix_element(a, i, j) + matrix_element(b, i, j);
		}
	}

	return 0;
}

int8_t matrix_multiply(Matrix a, Matrix b, Matrix *out)
{
	uint32_t i, j;
	float r[a.cols];
	float c[b.rows];
	Vector u, v;
	float dp;
	
	assert(a.cols == b.rows);
	assert(a.rows == out->rows);
	assert(b.cols == out->cols);

	new_vector(&u, a.cols, r);
	new_vector(&v, b.rows, c);
	
	for (i = 0; i < a.cols; i++) {
		row_vector(a, i, &u);
		for (j = 0; j < a.cols; j++) {
			column_vector(b, j, &v);
			dp = dot_product(u, v);
			assert(dp != NaN);
			matrix_element(*out, i, j) = dp;
		}
	}

	return 0;
}

int8_t matrix_inverse(Matrix in, Matrix *out)
{
	uint32_t i, j;
	float factor;

	assert(in.rows == out->rows);
	assert(in.cols == out->cols);
	assert(in.rows == in.cols);

	identity_matrix(out);

	for (i = 0; i < in.rows; i++) {
		factor = 1 / matrix_element(in, i, i);
		matrix_row_scale(&in, i, factor);
		matrix_row_scale(out, i, factor);

		for (j = 0; j < in.rows; j++) {
			if (i == j) continue;
			factor = matrix_element(in, j, i) * -1;
			matrix_row_scale(&in, i, factor);
			matrix_row_scale(out, i, factor);
			matrix_row_add(in, i, &in, j);
			matrix_row_add(*out, i, out, j);
			matrix_row_scale(&in, i, 1 / factor);
			matrix_row_scale(out, i, 1 / factor);
		}
	}

	return 0;
}
