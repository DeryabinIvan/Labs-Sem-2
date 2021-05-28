#pragma once

#include <stdlib.h>
#include <windows.h>

#include "field.h"
#include "snake.h"

int* calculatePath(field* f, snake* s, POINT start, POINT finish);