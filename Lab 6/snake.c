#include "snake.h"

#include <stdlib.h>
#include <windows.h>

void createSnake(snake* s, field* f) {
	s->length = 3;

	s->parts = (snake_part*) malloc(2 * sizeof(snake_part));
	if (s->parts == NULL) {
		return;
	}

	s->parts[0].sym = HEAD;

	for (int i = 0; i < s->length; i++) {
		if(i > 0)
			s->parts[i].sym = BODY;

		s->parts[i].direction = UP;
		s->parts[i].coord.x = f->height / 2;
		s->parts[i].coord.y = f->width / 2 + i;
	}
}

void deleteSnake(snake* s) {
	free(s->parts);
}

void addPart(snake* s) {
	s->length++;

	s->parts = (snake_part*) realloc(s->parts, s->length);
	if (s->parts == NULL) {
		return;
	}

	snake_part* tail = s->parts + s->length;
	snake_part* before_tail = s->parts + (s->length - 1);
	
	tail->sym = 's';
	tail->direction = before_tail->direction;
	
	switch (tail->direction) 	{
		case UP:
			tail->coord.y = before_tail->coord.y - 1;
			tail->coord.x = before_tail->coord.x;
			break;

		case DOWN:
			tail->coord.y = before_tail->coord.y + 1;
			tail->coord.x = before_tail->coord.x;
			break;

		case LEFT:
			tail->coord.y = before_tail->coord.y;
			tail->coord.x = before_tail->coord.x + 1;
			break;

		case RIGTH:
			tail->coord.y = before_tail->coord.y;
			tail->coord.x = before_tail->coord.x - 1;
			break;
	}

}

int findInterception(snake* s) {
	for (int i = 0; i < s->length - 1; i++)	{
		for (int j = 0; j < s->length; j++) 		{
			if (s->parts[i].coord.x == s->parts[j].coord.x ||
				s->parts[i].coord.y == s->parts[j].coord.y)
				return 1;
		}
	}

	return 0;
}

int moveSnake(snake* s, field* f, int dir) {
	for (int i = 0; i < s->length; i++) {
		snake_part* part = s->parts + i;

		switch (part->direction) {
			case UP:
				part->coord.y--;
				break;

			case DOWN:
				part->coord.y++;
				break;

			case LEFT:
				part->coord.x--;
				break;

			case RIGTH:
				part->coord.x++;
				break;
		}

		if (i == 0) {
			int cell_type = getXYType(f, part->coord.x, part->coord.y);
			switch (cell_type) {
				case BORDER_TL:
				case BORDER_DL:
				case BORDER_TR:
				case BORDER_DR:
				case BORDER_LEFT_RIGTH:
				case BORDER_TOP_DOWN:
				case WALL:
					return -1;

				case FOOD:
					addPart(s);
					break;
			}

			if (findInterception(s)) {
				return -1;
			}
		}
	}
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
		cli_coord.X = s->parts[i].coord.x + 1;
		cli_coord.Y = s->parts[i].coord.y + 1;

		SetConsoleCursorPosition(cli, cli_coord);
		printf("%c", s->parts[i].sym);
	}

	SetConsoleCursorPosition(cli, saved_coord);
}
