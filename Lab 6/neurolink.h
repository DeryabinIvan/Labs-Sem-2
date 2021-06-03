#pragma once

#include <stdlib.h>
#include <windows.h>

#include "field.h"
#include "snake.h"

//расчет маршрута с помощью волнового алгоритма
int calculatePath(field* f, snake* s, POINT start, POINT finish, int debug);

//простое перемещение в нужную сторону
int simpleMove(field* f, snake* s);