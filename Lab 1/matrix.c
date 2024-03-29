#include "matrix.h"

#include <malloc.h>
#include <stdlib.h>
#include <time.h>

void create_matrix(matrix *m, int val) {
	m->det = 0;
	m->det_flag = 0;

	//allocate memory
	m->data = (int**) malloc(sizeof(float *) * m->size);
	for (int i = 0; i < m->size; i++) {
		m->data[i] = (int*) malloc(sizeof(float) * m->size);
	}

	//fill matrix
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
	m->det_flag = 0;
}

void fill_random(matrix* m) {
	srand(time(0));

	if (m->data == NULL) {
		return;
	}

	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			m->data[i][j] = -100 + rand() % 200;
		}
	}
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

	m->det = 0;
	m->det_flag = 0;

	//get size
	fscanf_s(f, "%d", &(m->size));
	if (m->size < 0)
		return -1;

	create_matrix(m, 0);

	int ret = 0;
	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			ret = fscanf_s(f, "%d", &(m->data[i][j]));

			if (ret == EOF) {
				destroy_matrix(m);
				return -1;
			}
		}
	}

	return 1;
}

void fill_from_keyboard(matrix* m) {
	m->det = 0;
	m->det_flag = 0;

	if (m->size == 0) {
		printf("Enter matrix size: ");
		scanf_s("%d", &(m->size));
	}

	//allocate memory
	m->data = (int**) malloc(sizeof(float*) * m->size);
	for (int i = 0; i < m->size; i++) {
		m->data[i] = (int*) malloc(sizeof(float) * m->size);
	}

	printf("Enter matrix:\n");

	//fill matrix
	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			scanf_s("%d", &(m->data[i][j]));
		}
	}
}

void print_matrix(matrix *m) {
	for (int i = 0; i < m->size; i++) {
		for (int j = 0; j < m->size; j++) {
			printf("%d\t", m->data[i][j]);
		}
		printf("\n");
	}
}

void minor(const matrix *src, matrix *Mij, int row, int column) {
	Mij->size = src->size - 1;
	create_matrix(Mij, 0);

	for (int i = 0, ai = 0; i < src->size; i++) {
		if (i != row) {
			for (int j = 0, aj = 0; j < src->size; j++) {
				if (j != column) {
					Mij->data[ai][aj++] = src->data[i][j];
				}
			}
			ai++;
		}
	}
}

long determinant(matrix* m) {
	if (m->det_flag) {
		return m->det;
	}

	static int counter = 0;
	counter++;

	if (m->size == 1) {
		counter--;

		m->det_flag = 1;
		m->det = m->data[0][0];

		return m->det;
	} else if (m->size == 2) {
		counter--;

		m->det_flag = 1;
		m->det = m->data[0][0] * m->data[1][1] - m->data[1][0] * m->data[0][1];

		return m->det;
	}

	clock_t start = clock();

	long _det = 0;

	matrix Mij;
	for (int i = 0; i < m->size; i++) {
		minor(m, &Mij, 0, i);
		//print_matrix(&Mij);

		int minus = (i % 2 == 0) ? 1 : -1;

		_det += minus * m->data[0][i] * determinant(&Mij);
		
		destroy_matrix(&Mij);
	}

	if (counter == 1) {
		clock_t end = clock();
		double cpu_time = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("[CPU time: %.4f s]\n", cpu_time);

		m->det_flag = 1;
		m->det = _det;
	}

	counter--;
	return _det;
}
