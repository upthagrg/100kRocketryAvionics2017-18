#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include "vector.h"

typedef struct Matrix {
	uint32_t rows;
	uint32_t cols;
	float **matrix;
} Matrix;

void new_matrix(Matrix *, uint32_t rows, uint32_t cols, float [rows][cols]);

void identity_matrix(Matrix *);

int8_t row_vector(Matrix, uint32_t, Vector *);

int8_t column_vector(Matrix, uint32_t, Vector *);

int8_t matrix_scale(Matrix, float, Matrix *);

int8_t matrix_transpose(Matrix, Matrix *);

int8_t matrix_add(Matrix, Matrix, Matrix *);

int8_t matrix_multiply(Matrix, Matrix, Matrix *);

int8_t matrix_inverse(Matrix, Matrix *);

#endif
