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
	return 0;
}

void startGame() {
	srand(time(0));

	field f;
	f.height = 20;
	f.width = 20;
	craeteField(&f);

	snake s;
	createSnake(&s, &f);

	printField(&f);
	printSnake(&s);

	HANDLE cli = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD zero_coord;

	zero_coord.X = 0;
	zero_coord.Y = 0;

	int new_direction = NONE,
		snake_direction = getHeadDirection(&s);

	//игровой цикл
	do {
		SetConsoleCursorPosition(cli, zero_coord);

		printField(&f);
		printSnake(&s);

		printf("Score: %d\n", s.length);

		if (GetAsyncKeyState(VK_UP) & 1) {
			new_direction = UP;
		} else if (GetAsyncKeyState(VK_DOWN) & 1) {
			new_direction = DOWN;
		} else if (GetAsyncKeyState(VK_LEFT) & 1) {
			new_direction = LEFT;
		} else if (GetAsyncKeyState(VK_RIGHT) & 1) {
			new_direction = RIGTH;
		} else {
			new_direction = NONE;
		}

		snake_direction = getHeadDirection(&s);
		if (abs(snake_direction - new_direction) == 1) {
			new_direction = NONE;
		}

		Sleep(100);
	}while (moveSnake(&s, &f, new_direction));

	deleteField(&f);
	deleteSnake(&s);
}

void startAutoMode() {

}
