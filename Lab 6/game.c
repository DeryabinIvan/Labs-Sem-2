#include "game.h"

#include <stdio.h>
#include <time.h>
#include <windows.h>

#include "cli_graphics.h"

#include "field.h"
#include "snake.h"
#include "neurolink.h"

int getDirectionKey() {
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

	//для поиска пути
	f->food = food;

	return 1;
}

int addFoodMultiplayer(field* f, snake* s1, snake* s2) {
	POINT food;

	int counter = 0;

	do {
		food.x = 1 + rand() % f->height;
		food.y = 1 + rand() % f->width;

		if ((calculatePath(f, s1, getHeadCoord(s1), food, 0) == NONE) ||
			(calculatePath(f, s2, getHeadCoord(s2), food, 0) == NONE)) {
			counter++;
		}

		if (counter == 2000) {
			return 0;
		}
	} while (f->data[food.x][food.y] != EMPTY || 
			 existInParts(s1, food.x, food.y) ||
			 existInParts(s2, food.x, food.y)
			 );

	f->data[food.x][food.y] = FOOD;

	//для поиска пути
	f->food = food;

	return 1;
}

void clearSnakePlace(snake* s) {
	POINT saved_coord = getCursorPosistion(), sp_coord;

	setColor(0, FOREGROUND_WHITE);
	for (int i = 0; i < s->length; i++) {
		sp_coord.x = s->parts[i].coord.y;
		sp_coord.y = s->parts[i].coord.x;

		setCursorPosistion(&sp_coord);

		printf("%c", EMPTY);
	}

	setCursorPosistion(&saved_coord);
}

void printFood(POINT coord) {
	POINT saved_coord = getCursorPosistion(), cli_coord;

	cli_coord.x = coord.y;
	cli_coord.y = coord.x;

	setColor(0, FOREGROUND_GREEN);
	setCursorPosistion(&cli_coord);
	printf("%c", FOOD);

	setColor(0, FOREGROUND_WHITE);
	setCursorPosistion(&saved_coord);
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
	POINT saved_coord = {0, 0}, score_coord = {0, f.height+2};

	int new_direction = NONE, snake_direction = NONE;

	int status = 1;

	//игровой цикл
	do {
		getCursorPosistion(&saved_coord);

		//printField(&f);
		printSnake(&s);

		setCursorPosistion(&score_coord);
		printf("Score: %d\n", s.length);

		new_direction = getDirectionKey();

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
			printFood(f.food);
		}

		Sleep(100);

		clearSnakePlace(&s);
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
	POINT saved_coord = {0, 0}, score_coord = {0, f.height + 2};

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

		clearSnakePlace(&s);

		status = moveSnake(&s, &f, new_direction);

		setCursorPosistion(&saved_coord);

		//printField(&f);
		printSnake(&s);

		setCursorPosistion(&score_coord);
		printf("Score: %d\n", s.length);
		printDirection(new_direction);

		if (status == 2) {
			removeFood(&f);
			if (!addFood(&f, &s)) {
				printf("Game over!\n");
				break;
			}
			printFood(f.food);
		} else if (status == 0) {
			break;
		}

		Sleep(50);
	}

	deleteField(&f);
	deleteSnake(&s);

	Sleep(100);
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

	//назовем наших змеек Эвриала и Сфено
	snake stheno, euryale;
	createSnake(&stheno, &f);
	createSnake(&euryale, &f);

	//для Эвриалы поставим фиолетовый цвет
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

	POINT saved_coord = {0, 0},
		  score_coord = {0, f.height+2};

	int stheno_dir = NONE, euryale_dir = NONE;

	int stheno_status = 1, euryale_status = 1;

	//игровой цикл
	while (1) {
		//копируем актуальное поле
		for (int i = 0; i < f.height; i++) {
			memcpy(render.data[i], f.data[i], f.width * sizeof(unsigned char));
		}

		//"рендерим" змеек на поле
		for (int i = 0; i < stheno.length; i++) {
			render.data[stheno.parts[i].coord.x][stheno.parts[i].coord.y] = WALL;
		}
		for (int i = 0; i < euryale.length; i++) {
			render.data[euryale.parts[i].coord.x][euryale.parts[i].coord.y] = WALL;
		}

		//считаем пути для каждой отдельно
		//причем учитываем положение соперника
		stheno_dir = calculatePath(&render, &stheno, getHeadCoord(&stheno), f.food, 0);
		euryale_dir = calculatePath(&render, &euryale, getHeadCoord(&euryale), f.food, 0);
		if (stheno_dir == NONE && euryale_dir == NONE) {
			printf("Game over! [Can't find place for food]\n");
			break;
		}

		if (euryale_dir == NONE) {
			euryale_dir = simpleMove(&render, &euryale);
		}
		if (stheno_dir == NONE) {
			stheno_dir = simpleMove(&render, &stheno);
		}

		if (checkDirection(getHeadDirection(&stheno), stheno_dir)) {
			stheno_dir = NONE;
		}
		if (checkDirection(getHeadDirection(&euryale), euryale_dir)) {
			euryale_dir = NONE;
		}

		clearSnakePlace(&stheno);
		clearSnakePlace(&euryale);

		stheno_status = moveSnake(&stheno, &f, stheno_dir);
		euryale_status = moveSnake(&euryale, &f, euryale_dir);

		setCursorPosistion(&saved_coord);

		//printField(&f);
		printSnake(&stheno);
		printSnake(&euryale);

		setCursorPosistion(&score_coord);
		setColor(stheno.color, FOREGROUND_WHITE);
		printf("Stheno");
		setColor(0, FOREGROUND_WHITE);
		printf(": %d\n", stheno.length);
		setColor(euryale.color, FOREGROUND_WHITE);
		printf("Euryale");
		setColor(0, FOREGROUND_WHITE);
		printf(": %d\n", euryale.length);

		if (stheno_status == 2 || euryale_status == 2) {
			removeFood(&f);
			if (!addFoodMultiplayer(&f, &stheno, &euryale)) {
				printf("Game over!\n");
				break;
			}
			printFood(f.food);
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
	deleteField(&render);

	deleteSnake(&stheno);
	deleteSnake(&euryale);

	Sleep(100);
}
