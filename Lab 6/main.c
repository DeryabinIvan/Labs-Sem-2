#include <stdlib.h>
#include <stdio.h>

#include "game.h"

enum MENU{START=1, AUTO, AUTO_DEBUG, AUTO_MULTIPLAYER, HELP, EXIT};

int main() {
	int menu_key, difficulty;

	while (1){
		printf("Menu:\
				\n\t%d) Solo\
				\n\t%d) Auto mode\
				\n\t%d) Auto with debug information\
				\n\t   (use fullscreen for best results) \
				\n\t%d) Auto mode with 2 snek\
				\n\t%d) Help\
				\n\t%d) Exit\n", 
			   START, AUTO, AUTO_DEBUG, AUTO_MULTIPLAYER, HELP, EXIT);

		scanf_s("%d", &menu_key);
		system("cls");

		switch (menu_key){
			case START:
				selectDifficulty(&difficulty);
				hideCursor();
				startGame(difficulty);
				break;

			case AUTO:
				selectDifficulty(&difficulty);
				hideCursor();
				startAutoMode(difficulty, 0);
				break;

			case AUTO_DEBUG:
				selectDifficulty(&difficulty);
				hideCursor();
				startAutoMode(difficulty, 1);
				break;

			case AUTO_MULTIPLAYER:
				selectDifficulty(&difficulty);
				hideCursor();
				startAutoMultiplayer(difficulty);
				break;

			case HELP:
				printf("WASD - control\n");
				printf("Copyrigth: Alex Link\n");
				break;

			case EXIT:
				return 0;
		}
	}
}
