#include "stdafx.h"

void character_move(Character* chr, Direction dir)
{
	switch (dir)
	{
	case Direction::UP:
		// 이동 가능 여부 확인
		if (Check_Move(chr->x, chr->y - 1) == true)
		{
			chr->y--;
		}
		break;
	case Direction::DOWN:
		// 이동 가능 여부 확인
		if (Check_Move(chr->x, chr->y + 1) == true)
		{
			chr->y++;
		}
		break;
	case Direction::LEFT:
		// 이동 가능 여부 확인
		if (Check_Move(chr->x - 1, chr->y) == true)
		{
			chr->x--;
		}
		break;
	case Direction::RIGHT:
		// 이동 가능 여부 확인
		if (Check_Move(chr->x + 1, chr->y) == true)
		{
			chr->x++;
		}
		break;
	}
}

// 캐릭터 시작 좌표, 컨트롤러 번호, 패드를 쓸건지 여부
void character_init(Character* chr, const int x, const int y, const int control_number, const bool is_pad, int player_count)
{
	chr->x = x;
	chr->y = y;
	chr->health = 100;
	chr->number = player_count;
	chr->bomb_level = 1;
	chr->bomb_count = 0;
	chr->bomb_max_count = 1;

	set_button(chr, control_number, is_pad);
}

// 캐릭터 그리기
void character_render(Character* chr, int x_mul, int y_mul, const char* player, WORD bg, WORD fg)
{
	char buf[16] = "";

	// 가로 길이만큼 player 문자를 버퍼에 채운다.
	for (int i = 0; i < x_mul / 2; i++)
	{
		strcat_s(buf, 16, player);
	}

	// 세로 길이만큼 출력한다.
	for (int i = 0; i < y_mul; i++)
	{
		LHPutAttrCharEx(chr->x * x_mul, chr->y * y_mul + i, buf, bg, fg);
	}
}