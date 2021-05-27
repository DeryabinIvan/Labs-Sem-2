#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "field.h"
#include "snake.h"

enum MENU{START=1, AUTO, HELP, EXIT};

void startGame();
void startAutoMode();

int main() {
	int menu_key;

	while (1){
		printf("Menu:\
				\n\t%d) New Game\
				\n\t%d) Auto mode\
				\n\t%d) Help\
				\n\t%d) Exit\n", 
			   START, AUTO, HELP, EXIT);

		scanf_s("%d", &menu_key);
		system("cls");

		switch (menu_key){
			case START:
				startGame();
				break;
			case AUTO:
				startAutoMode();
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

void startGame() {
	srand(time(0));

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

	int new_direction = NONE,
		snake_direction = getHeadDirection(&s);

	//игровой цикл
	do {
		SetConsoleCursorPosition(cli, saved_coord);

		printField(&f);
		printSnake(&s);

		printf("Score: %d\n", s.length);

		new_direction = getDirection();

		snake_direction = getHeadDirection(&s);
		if (abs(snake_direction - new_direction) == 1) {
			new_direction = NONE;
		}

		Sleep(100);
	}while (moveSnake(&s, &f, new_direction));

	deleteField(&f);
	deleteSnake(&s);

	Sleep(100);
}

void startAutoMode() {

}
