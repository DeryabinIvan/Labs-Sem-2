#pragma once

#include <stdlib.h>

//соло режим
void startGame(int difficulty);

//автоматический соло режим
void startAutoMode(int difficulty, int debug);

//автоматический режим с 2мя змейками
void startAutoMultiplayer(int difficulty);

//выбор сложности
void selectDifficulty(int* diff);