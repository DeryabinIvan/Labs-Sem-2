#pragma once

#include <stdio.h>
#include <memory.h>

typedef struct m{
	float **data;
	int size;
} matrix;

void create_matrix(matrix *m, float val);

void destroy_matrix(matrix *m);

void copy_matrix(matrix *a, matrix *b);

int read_from_file(FILE *f, matrix *m);

void print_matrix(matrix *m);

