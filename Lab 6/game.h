#pragma once

#include <stdlib.h>

//���� �����
void startGame(int difficulty);

//�������������� ���� �����
void startAutoMode(int difficulty, int debug);

//�������������� ����� � 2�� ��������
void startAutoMultiplayer(int difficulty);

//����� ���������
void selectDifficulty(int* diff);