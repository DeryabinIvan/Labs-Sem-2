#include "matrix.h"

#include <malloc.h>

void create_matrix(matrix *m, float val) {
	//allocate memory
	m->data = (float **) malloc(sizeof(float *) * m->size);
	for (int i = 0; i < m->size; i++) {
		m->data[i] = (float *) malloc(sizeof(float) * m->size);
	}

	//fill matrix 0
	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			m->data[i][j] = val;
		}
	}
}

void destroy_matrix(matrix *m) {
	if (m == NULL) return;

	for (int i = 0; i < m->size; i++)
		free(m->data[i]);
	
	free(m->data);

	m->size = -1;
}

void copy_matrix(matrix *a, matrix *b) {
	if (a == NULL || b == NULL || a->size != b->size)
		return;

	for (int i = 0; i < a->size; i++) {
		for (int j = 0; j < a->size; j++) {
			b->data[i][j] = a->data[i][j];
		}
	}
}

int read_from_file(FILE *f, matrix *m) {
	if (f == NULL || m == NULL)
		return -1;

	//get size
	fscanf_s(f, "%d", &(m->size));
	if (m->size < 0)
		return -1;

	create_matrix(m);

	int ret = 0;
	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			ret = fscanf_s(f, "%f", &(m->data[i][j]));

			if (ret == EOF) {
				destroy_matrix(m);
				return -1;
			}
		}
	}

	return 1;
}

void print_matrix(matrix *m) {
	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			printf("%.2f ", m->data[i][j]);
		}
		printf("\n");
	}
}
