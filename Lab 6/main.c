#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "field.h"
#include "snake.h"
#include "neurolink.h"

enum MENU{START=1, AUTO, AUTO_DEBUG, HELP, EXIT};

void startGame();
void startAutoMode(int debug);

int main() {
	int menu_key;

	while (1){
		printf("Menu:\
				\n\t%d) New Game\
				\n\t%d) Auto mode\
				\n\t%d) Auto with debug information\
				\n\t   (use fullscreen to best result) \
				\n\t%d) Help\
				\n\t%d) Exit\n", 
			   START, AUTO, AUTO_DEBUG, HELP, EXIT);

		scanf_s("%d", &menu_key);
		system("cls");

		switch (menu_key){
			case START:
				startGame();
				break;
			case AUTO:
				startAutoMode(0);
				break;
			case AUTO_DEBUG:
				startAutoMode(1);
				break;
			case HELP:
				printf("WASD - control; X or ESC-exit\nThat`s all ;)\n");
				break;
			case EXIT:
				return 0;
		}
	}
}

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

void disableConsoleCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = 0;
	SetConsoleCursorInfo(out, &cursorInfo);
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
	
	//для поиска пути
	f->food = food;
}

void startGame() {
	srand((unsigned int) time(0));

	disableConsoleCursor();

	field f;
	f.height = 20;
	f.width = 20;
	craeteField(&f);

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

	//игровой цикл
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
			if (!addFood(&f, &s)) {
				printf("Game over!\n");
				break;
			}
		}

		Sleep(100);
	}while (status = moveSnake(&s, &f, new_direction));

	deleteField(&f);
	deleteSnake(&s);

	Sleep(100);
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

void startAutoMode(int debug) {
	srand((unsigned int) time(0));

	disableConsoleCursor();

	field f;
	f.height = 20;
	f.width = 20;
	craeteField(&f);

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

	//игровой цикл
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
