#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include "vector.h"

typedef struct Matrix {
	uint32_t rows;
	uint32_t cols;
	float **matrix;
} Matrix;

/*
  Initializes the matrix `matrix' to be a `rows'x`cols' matrix stored in `arr'
 */
void new_matrix(Matrix *matrix, uint32_t rows, uint32_t cols,
                float arr[rows][cols]);

/*
  Macro to access the element at row `r' column `c' of matrix `m'
  Can be used to get or set the value
 */
#define matrix_element(m, r, c) ((float (*)[(m).cols])(m).matrix)[r][c]

/*
  Writes an identity matrix to the matrix pointed at by `matrix'
 */
void identity_matrix(Matrix *matrix);

/*
  Returns the row `row' of the matrix `matrix' in the vector `out'
 */
int8_t row_vector(Matrix matrix, uint32_t row, Vector *out);

/*
  Returns the column `column' of the matrix `matrix' in the vector `out'
 */
int8_t column_vector(Matrix matrix, uint32_t column, Vector *out);

/*
  Adds the row `row1' of the matrix `in' to the row `row2' of the matrix `out'
 */
int8_t matrix_row_add(Matrix in, uint32_t in_row, Matrix *out, uint32_t out_row);

/*
  Scales every element in the row `row' of the matrix `matrix' by `factor'
 */
int8_t matrix_row_scale(Matrix *matrix, uint32_t row, float factor);

/*
  Scales the matrix `matrix' by `factor'
 */
int8_t matrix_scale(Matrix *matrix, float factor);

/*
  Finds the transpose of the matrix `in' and stores the result in `out'
 */
int8_t matrix_transpose(Matrix in, Matrix *out);

/*
  Adds the matrix `a' to the matrix `b' and stores the result in `out'
 */
int8_t matrix_add(Matrix a, Matrix b, Matrix *out);

/*
  Multiplys the matrix `a' by the matrix `b' and stores the result in `out'
 */
int8_t matrix_multiply(Matrix a, Matrix b, Matrix *out);

/*
  Finds the inverse of the matrix `in' and stores the result in `out'
 */
int8_t matrix_inverse(Matrix in, Matrix *out);

#endif
