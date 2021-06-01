#include "game.h"

#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "cli_graphics.h"

#include "field.h"
#include "snake.h"
#include "neurolink.h"

int getDirection() {
	if (GetAsyncKeyState(VK_UP) & 1) {
		return UP;
	} else if (GetAsyncKeyState(VK_DOWN) & 1) {
		return DOWN;
	} else if (GetAsyncKeyState(VK_LEFT) & 1) {
		return LEFT;
	} else if (GetAsyncKeyState(VK_RIGHT) & 1) {
		return RIGTH;
	}

	return NONE;
}

int checkDirection(int snake_dir, int new_dir) {
	return (snake_dir + new_dir) == 0;
}

int addFood(field* f, snake* s) {
	POINT food;

	int counter = 0;

	do {
		food.x = 1 + rand() % f->height;
		food.y = 1 + rand() % f->width;

		counter++;
		if (counter == 2000) {
			return 0;
		}
	} while (f->data[food.x][food.y] != EMPTY || existInParts(s, food.x, food.y) ||
			 (calculatePath(f, s, getHeadCoord(s), food, 0) == NONE));

	f->data[food.x][food.y] = FOOD;

	//��� ������ ����
	f->food = food;

	return 1;
}

void printDirection(int dir) {
	switch (dir) {
		case UP:
			printf("Up   ");
			break;
		case DOWN:
			printf("Down ");
			break;
		case LEFT:
			printf("Left ");
			break;
		case RIGTH:
			printf("Rigth");
			break;
		default:
			printf("None ");
	}
	printf("\n");
}

void selectDifficulty(int* diff) {
#define MIN_DIFFICULTY 5
#define MAX_DIFFICULTY 20

	printf("Enter difficulty (%d - %d): ", MIN_DIFFICULTY, MAX_DIFFICULTY);
	scanf_s("%d", diff);

	if (*diff < 0) {
		*diff = MIN_DIFFICULTY;
	} else if (*diff > MAX_DIFFICULTY) {
		*diff = MAX_DIFFICULTY;
	}

	system("cls");
}

void startGame(int difficulty) {
	srand((unsigned int) time(0));

	field f;
	f.height = 20;
	f.width = 20;
	createField(&f, difficulty);

	snake s;
	createSnake(&s, &f);
	addFood(&f, &s);

	printField(&f);
	printSnake(&s);

	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD saved_coord;

	saved_coord.X = 0;
	saved_coord.Y = 0;

	int new_direction = NONE, snake_direction = NONE;

	int status = 1;

	//������� ����
	do {
		SetConsoleCursorPosition(cli, saved_coord);

		printField(&f);
		printSnake(&s);

		printf("Score: %d\n", s.length);

		new_direction = getDirection();

		snake_direction = getHeadDirection(&s);
		if (checkDirection(snake_direction, new_direction)) {
			new_direction = NONE;
		}

		if (status == 2) {
			removeFood(&f);
			if (!addFood(&f, &s)) {
				printf("Game over!\n");
				break;
			}
		}

		Sleep(100);
	} while (status = moveSnake(&s, &f, new_direction));

	deleteField(&f);
	deleteSnake(&s);

	Sleep(100);
}

void startAutoMode(int difficulty, int debug) {
	srand((unsigned int) time(0));

	field f;
	f.height = 20;
	f.width = 20;
	createField(&f, difficulty);

	snake s;
	createSnake(&s, &f);
	addFood(&f, &s);

	printField(&f);
	printSnake(&s);

	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD saved_coord;

	saved_coord.X = 0;
	saved_coord.Y = 0;

	int new_direction = NONE, snake_direction = NONE;

	int status = 1;
	int path_len = 0;

	//������� ����
	while (1) {
		new_direction = calculatePath(&f, &s, getHeadCoord(&s), f.food, debug);
		if (new_direction == NONE) {
			printf("Game over! Score: %d\n", s.length);
			break;
		}

		snake_direction = getHeadDirection(&s);

		if (checkDirection(snake_direction, new_direction)) {
			new_direction = NONE;
		}

		status = moveSnake(&s, &f, new_direction);

		SetConsoleCursorPosition(cli, saved_coord);

		printField(&f);
		printSnake(&s);

		printf("Score: %d\n", s.length);
		printDirection(new_direction);

		if (status == 2) {
			removeFood(&f);
			if (!addFood(&f, &s)) {
				printf("Game over!\n");
				break;
			}
		} else if (status == 0) {
			break;
		}

		Sleep(50);
	}

	deleteField(&f);
	deleteSnake(&s);

	Sleep(100);
}

int addFoodMultiplayer(field* f, snake* s1, snake* s2) {
	POINT food;

	int counter = 0;

	do {
		food.x = 1 + rand() % f->height;
		food.y = 1 + rand() % f->width;

		counter++;
		if (counter == 2000) {
			return 0;
		}
	} while (f->data[food.x][food.y] != EMPTY || 
			 existInParts(s1, food.x, food.y) ||
			 existInParts(s2, food.x, food.y) ||
			 (calculatePath(f, s1, getHeadCoord(s1), food, 0) == NONE) ||
			 (calculatePath(f, s2, getHeadCoord(s2), food, 0) == NONE)
			 );

	f->data[food.x][food.y] = FOOD;

	//��� ������ ����
	f->food = food;

	return 1;
}

void startAutoMultiplayer(int difficulty) {
	srand((unsigned int) time(0));

	field f, render;
	f.height = 25;
	f.width = 25;

	render.height = f.height;
	render.width = f.width;

	createField(&f, difficulty);
	createField(&render, 0);

	//������� ����� ����� ������� � �����
	snake stheno, euryale;
	createSnake(&stheno, &f);
	createSnake(&euryale, &f);

	//��� ������� �������� ���������� ����
	euryale.color = BACKGROUND_BLUE | BACKGROUND_RED;
	for (int i = 0; i < euryale.length; i++) {
		euryale.parts[i].coord.y = f.width-1;
		
		if (getCellXY(&f, euryale.parts[i].coord.x, euryale.parts[i].coord.y) == WALL) {
			f.data[euryale.parts[i].coord.x][euryale.parts[i].coord.y] = EMPTY;
		}
	}

	addFoodMultiplayer(&f, &stheno, &euryale);

	printField(&f);
	printSnake(&stheno);
	printSnake(&euryale);

	POINT saved_coord = {0, 0};

	int new_dir_s = NONE, new_dir_e = NONE;

	int stheno_status = 1, euryale_status = 1;

	//������� ����
	while (1) {
		//�������� ���������� ����
		for (int i = 0; i < f.height; i++) {
			memcpy(render.data[i], f.data[i], f.width * sizeof(unsigned char));
		}

		//"��������" ����� �� ����
		for (int i = 0; i < stheno.length; i++) {
			render.data[stheno.parts[i].coord.x][stheno.parts[i].coord.y] = WALL;
		}
		for (int i = 0; i < euryale.length; i++) {
			render.data[euryale.parts[i].coord.x][euryale.parts[i].coord.y] = WALL;
		}

		//������� ���� ��� ������ ��������
		//������ ��������� ��������� ���������
		new_dir_s = calculatePath(&render, &stheno, getHeadCoord(&stheno), f.food, 0);
		new_dir_e = calculatePath(&render, &euryale, getHeadCoord(&euryale), f.food, 0);
		if (new_dir_s == NONE || new_dir_e == NONE) {
			printf("Game over!\n");
			break;
		}

		if (checkDirection(getHeadDirection(&stheno), new_dir_s)) {
			new_dir_s = NONE;
		}
		if (checkDirection(getHeadDirection(&euryale), new_dir_e)) {
			new_dir_e = NONE;
		}

		stheno_status = moveSnake(&stheno, &f, new_dir_s);
		euryale_status = moveSnake(&euryale, &f, new_dir_e);

		setCursorPosistion(&saved_coord);

		printField(&f);
		printSnake(&stheno);
		printSnake(&euryale);

		printf("Stheno: %d\nEuryale: %d\n", stheno.length, euryale.length);

		if (stheno_status == 2 || euryale_status == 2) {
			removeFood(&f);
			if (!addFoodMultiplayer(&f, &stheno, &euryale)) {
				printf("Game over!\n");
				break;
			}
		} else if (stheno_status == 0 || euryale_status == 0) {
			break;
		}

		Sleep(50);
	}
	if (stheno_status == 0) {
		printf("Stheno lose\n");
	} else if (euryale_status == 0) {
		printf("Euryale lose\n");
	}

	deleteField(&f);
	deleteSnake(&stheno);
	deleteSnake(&euryale);

	Sleep(100);
}
