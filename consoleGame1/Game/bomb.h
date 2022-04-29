#pragma once

struct Bomb
{
	char player;
	int x;
	int y;
	int power;
	long int start_time;
	int is_on;
};

extern struct Bomb BombArr[16];

void Bomb_Init();
void create_bomb(Character* chr);
void Bomb_Update();