#include "neurolink.h"

int** im_num_space;

void fill_space(int x, int y, int len) {
	if (im_num_space[x][y] == INT_MAX) {
		return;
	}

	if (im_num_space[x][y] == -1) {
		im_num_space[x][y] = len;
		return;
	}

	if (im_num_space[x + 1][y] == 0) {
		im_num_space[x + 1][y] = len;
		fill_space(x + 1, y, len+1);
	} else if (im_num_space[x - 1][y] == 0) {
		im_num_space[x - 1][y] = len;
		fill_space(x - 1, y, len + 1);
	} else if (im_num_space[x][y + 1] == 0) {
		im_num_space[x][y + 1] = len;
		fill_space(x, y + 1, len + 1);
	} else if (im_num_space[x][y - 1] == 0) {
		im_num_space[x][y] = len;
		fill_space(x, y - 1, len + 1);
	}
}

int* calculatePath(field* f, snake* s, POINT start, POINT finish) {
	//инициализация волнового алгоритма
	im_num_space = (int**) malloc(f->height * sizeof(int*));
	if (im_num_space == NULL) {
		exit(-1);
	}

	for (int i = 0; i < f->width; i++) {
		im_num_space[i] = (int*) malloc(f->width * sizeof(int));
		if (im_num_space[i] == NULL) {
			exit(-1);
		}
	}

	for (int i = 0; i < f->width; i++) {
		for (int j = 0; j < f->width; j++) {
			if (f->data[i + 1][j + 1] == WALL || existInParts(s, i, j)) {
				//стена или часть змейки это недосягаемые области
				im_num_space[i][j] = INT_MAX;
			} else {
				im_num_space[i][j] = 0;
			}
		}
	}
	im_num_space[finish.x][finish.y] = -1;

	//начало волнового алгоритма
	//заполняем поле
	fill_space(start.x, start.y, 0);

	int x = finish.x, y = finish.y;
	int path_len = im_num_space[x][y];
	
	int* path = (int*) malloc(path_len * sizeof(int));

	//развертка пути от финиша к старту
	for (int i = 0; i < path_len; i++) {
		if (im_num_space[x + 1][y] < im_num_space[x][y]) {
			path[i] = UP;
		} else if (im_num_space[x - 1][y] < im_num_space[x][y]) {
			path[i] = DOWN;
		} else if (im_num_space[x][y + 1] < im_num_space[x][y]) {
			path[i] = LEFT;
		} else if (im_num_space[x][y - 1] < im_num_space[x][y]) {
			path[i] = RIGTH;
		} else {
			path[i] = NONE;
		}
	}

	for (int i = 0; i < f->height; i++) {
		free(im_num_space[i]);
	}
	free(im_num_space);

	return path;
}
