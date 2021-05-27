// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "snake.h"

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

void createSnake(snake* s, field* f) {
	s->length = 3;

	s->parts = (snake_part*) malloc(s->length * sizeof(snake_part));
	if (s->parts == NULL) {
		return;
	}

	s->parts[0].sym = HEAD;

	for (int i = 0; i < s->length; i++) {
		if(i > 0)
			s->parts[i].sym = BODY;

		s->parts[i].direction = DOWN;
		s->parts[i].coord.x = s->length - i - 1;
		s->parts[i].coord.y = 0;
	}

	s->parts[s->length - 1].sym = '^';
}

void deleteSnake(snake* s) {
	free(s->parts);
}

void addPart(snake* s) {
	s->length++;

	snake_part* tmp = realloc(s->parts, s->length * sizeof(snake_part));
	if (tmp == NULL) {
		exit(-1);
	}
	s->parts = tmp;

	snake_part* new_tail = s->parts + (s->length - 1);
	snake_part* old_tail = s->parts + (s->length - 2);
	
	new_tail->direction = old_tail->direction;
	new_tail->coord.x = old_tail->coord.x;
	new_tail->coord.y = old_tail->coord.y;

	old_tail->sym = BODY;
	
	switch (new_tail->direction) {
		case UP:
			new_tail->coord.x--;
			break;

		case DOWN:
			new_tail->coord.x++;
			break;

		case LEFT:
			new_tail->coord.y++;
			break;

		case RIGTH:
			new_tail->coord.y--;
			break;
	}
}

int getHeadDirection(snake* s) {
	return s->parts[0].direction;
}

int findInterception(snake* s) {
	for (int j = 1; j < s->length; j++) {
		if (s->parts[0].coord.x == s->parts[j].coord.x &&
			s->parts[0].coord.y == s->parts[j].coord.y)
			return 1;
	}

	return 0;
}

void movePart(snake_part* part){
	switch (part->direction) {
		case UP:
			part->coord.x--;
			break;

		case DOWN:
			part->coord.x++;
			break;

		case LEFT:
			part->coord.y--;
			break;

		case RIGTH:
			part->coord.y++;
			break;
	}
}

int moveSnake(snake* s, field* f, int new_dir) {
	//двигаем голову...
	snake_part* head = s->parts;
	int current_dir = head->direction;

	if (new_dir != NONE) {
		head->direction = new_dir;
	}

	movePart(head);

	//...и проверяем
	int cell_type = getXYType(f, head->coord.x, head->coord.y);

	if (cell_type != FOOD) {
		//...на пересечения со стеной
		switch (cell_type) {
			case BORDER_TL:
			case BORDER_DL:
			case BORDER_TR:
			case BORDER_DR:
			case BORDER_LEFT_RIGTH:
			case BORDER_TOP_DOWN:
			case WALL:
			case -1:
				if (cell_type != -1) {
					printf("You killed by: %c\n", cell_type);
				} else {
					printf("Out of range\n");
				}
				return 0;
		}

		//...или с собой
		if (findInterception(s)) {
			printf("Interceptions not allowed!\n");
			return 0;
		}
	}

	int tmp_dir = NONE;

	//двигаем остальные части змейки
	for (int i = 1; i < s->length; i++) {
		snake_part* part = s->parts + i;

		tmp_dir = part->direction;
		part->direction = current_dir;
		current_dir = tmp_dir;

		movePart(part);
	}

	if (cell_type == FOOD) {
		removeFood(f, head->coord.x, head->coord.y);
		addPart(s);
		addFood(f);
	}

	//меняем направление хвоста
	snake_part* tail = s->parts + (s->length - 1);
	switch (tail->direction) {
		case UP:
			tail->sym = 'v';
			break;
		case DOWN:
			tail->sym = '^';
			break;
		case LEFT:
			tail->sym = '>';
			break;
		case RIGTH:
			tail->sym = '<';
			break;
	}

	return 1;
}

void printSnake(snake* s) {
	HANDLE cli =  GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cli_coord, saved_coord;

	CONSOLE_SCREEN_BUFFER_INFO cbi;
	GetConsoleScreenBufferInfo(cli, &cbi);
	saved_coord = cbi.dwCursorPosition;

	cli_coord.X = 0;
	cli_coord.Y = 0;

	SetConsoleCursorPosition(cli, cli_coord);

	for (int i = 0; i < s->length; i++) {
		cli_coord.X = s->parts[i].coord.y + 1;
		cli_coord.Y = s->parts[i].coord.x + 1;

		SetConsoleCursorPosition(cli, cli_coord);
		printf("%c", s->parts[i].sym);
	}

	SetConsoleCursorPosition(cli, saved_coord);
}
