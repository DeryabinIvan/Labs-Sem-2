#pragma once

#include <stdlib.h>

typedef struct f{
	unsigned char** data;
	int height, width;
} field;

enum FIELD_CELLS {
	//������� ����
	BORDER_TOP_DOWN = 205,
	BORDER_LEFT_RIGTH = 186,
	BORDER_TR = 188,
	BORDER_DR = 187,
	BORDER_TL = 201,
	BORDER_DL = 200,

	//������
	WALL = 4,

	//���
	FOOD = 15,

	//������ ������
	EMPTY = 32,
};

//������� ������� ����
void craeteField(field* f);

//������� ������� ���� � �������� ������
void deleteField(field* f);

//�������� ��� �� ����
void addFood(field* f);

//������� ���� �� �����
void printField(field* f);

//������� ��� ������
int getXYType(field* f, int x, int y);

//������ ��� � ����
void removeFood(field* f, int x, int y);