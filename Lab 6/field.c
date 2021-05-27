#include "field.h"

#include <stdlib.h>
#include <time.h>

const int FIELD_BORDER = 1;

void craeteField(field* f) {
	f->data = (unsigned char**) malloc((f->height + 2 * FIELD_BORDER) * sizeof(unsigned char*));
	if (f->data == NULL) {
		exit(-1);
	}

	for (int i = 0; i < f->width + 2 * FIELD_BORDER; i++) {
		f->data[i] = (unsigned char*) calloc((f->width + 2 * FIELD_BORDER + 1), sizeof(unsigned char));
		if (f->data[i] == NULL) {
			exit(-1);
		}

		if (i == 0 || i == f->height + FIELD_BORDER) {
			memset(f->data[i], BORDER_TOP_DOWN, f->width + FIELD_BORDER * 2);
		} else {
			memset(f->data[i], EMPTY, f->width + FIELD_BORDER * 2);
		}

		f->data[i][0] = BORDER_LEFT_RIGTH;
		f->data[i][f->width + FIELD_BORDER] = BORDER_LEFT_RIGTH;
		f->data[i][f->width + FIELD_BORDER + 1] = '\0';
	}
	f->data[0][0] = BORDER_TL;
	f->data[f->height + FIELD_BORDER][0] = BORDER_DL;
	f->data[0][f->width + FIELD_BORDER] = BORDER_DR;
	f->data[f->height + FIELD_BORDER][f->width + FIELD_BORDER] = BORDER_TR;

	//заполняем поле препятсвиями
	for (int i = 0; i < (f->height * f->width * 0.01); i++) {
		f->data[FIELD_BORDER + rand() % f->height][FIELD_BORDER + rand() % f->width] = WALL;
	}
}

void deleteField(field* f) {
	for (int i = 0; i < f->height + 2 * FIELD_BORDER; i++) {
		free(f->data[i]);
	}
	free(f->data);
}

void addFood(field* f, snake* s) {
	int x, y;

	do {
		x = FIELD_BORDER + rand() % f->height;
		y = FIELD_BORDER + rand() % f->width;
	}while(f->data[x][y] != EMPTY && existInParts(s, x, y));
	
	f->data[x][y] = FOOD;
}

void printField(field* f) {
	for (int i = 0; i < f->height + FIELD_BORDER * 2; i++) {
		printf("%s\n", f->data[i]);
	}
}

int getXYType(field* f, int x, int y) {
	if (x < -1 || y < -1 || x > f->height || y > f->width) {
		return -1;
	}
	return f->data[x + FIELD_BORDER][y + FIELD_BORDER];
}

void removeFood(field* f, int x, int y) {
	f->data[x + FIELD_BORDER][y + FIELD_BORDER] = EMPTY;
}
