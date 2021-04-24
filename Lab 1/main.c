#include <stdio.h>
#include "matrix.h"

int main() {
	FILE* f;
	fopen_s(&f, "matrix", "r");

	matrix a;
	a.size = 10;
	create_matrix(&a, 0);

	fill_random(&a);
	print_matrix(&a);

	int _det = determinant(&a);
	printf("\n|a| = %d\n", _det);

	_det = determinant(&a);
	printf("\n|a| = %d\n", _det);

	destroy_matrix(&a);

	system("pause");
	return 0;
}