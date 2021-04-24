#pragma once

#include <stdio.h>
#include <memory.h>

typedef struct m{
	int **data;
	int size;

	char det_flag;
	long det;
} matrix;

void create_matrix(matrix *m, int val);
void copy_matrix(matrix *a, matrix *b);
void destroy_matrix(matrix *m);

void fill_random(matrix* m);
void fill_from_keyboard(matrix* m);
int read_from_file(FILE *f, matrix *m);

void print_matrix(matrix *m);

long determinant(matrix *m);
