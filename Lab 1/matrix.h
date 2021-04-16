#pragma once

#include <stdio.h>
#include <memory.h>

typedef struct m{
	int **data;
	int size;
} matrix;

void create_matrix(matrix *m, int val);

void fill_random(matrix* m);

void destroy_matrix(matrix *m);

void copy_matrix(matrix *a, matrix *b);

int read_from_file(FILE *f, matrix *m);

void print_matrix(matrix *m);

long determinant(matrix *m);