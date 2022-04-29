#pragma once

typedef struct
{
	short	up;
	short	down;
	short	left;
	short	right;
	short	select;
	bool	gamepad;
}Character_control;

typedef struct
{
	char	number;
	int		x;
	int		y;
	int		health;
	int		bomb_level;
	int		bomb_count;
	int		bomb_max_count;
	Character_control ctl;
}Character;

enum class Direction
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
};

void character_move(Character* chr, Direction dir);
void character_init(Character* chr, const int x, const int y, const int control_number, const bool is_pad, int player_count);
void character_render(Character* chr, int x_mul, int y_mul, const char* player, WORD bg, WORD fg);