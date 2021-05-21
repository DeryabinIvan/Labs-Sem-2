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

	//игровой цикл
	while (moveSnake(&s, &f, NONE)) {
		SetConsoleCursorPosition(cli, zero_coord);

		printField(&f);
		printSnake(&s);

		Sleep(150);
	}

	deleteField(&f);
	deleteSnake(&s);
}

void startAutoMode() {

}
