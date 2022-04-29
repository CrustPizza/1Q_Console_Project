#include "stdafx.h"

void character_move(Character* chr, Direction dir)
{
	switch (dir)
	{
	case Direction::UP:
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x, chr->y - 1) == true)
		{
			chr->y--;
		}
		break;
	case Direction::DOWN:
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x, chr->y + 1) == true)
		{
			chr->y++;
		}
		break;
	case Direction::LEFT:
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x - 1, chr->y) == true)
		{
			chr->x--;
		}
		break;
	case Direction::RIGHT:
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x + 1, chr->y) == true)
		{
			chr->x++;
		}
		break;
	}
}

// ĳ���� ���� ��ǥ, ��Ʈ�ѷ� ��ȣ, �е带 ������ ����
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

// ĳ���� �׸���
void character_render(Character* chr, int x_mul, int y_mul, const char* player, WORD bg, WORD fg)
{
	char buf[16] = "";

	// ���� ���̸�ŭ player ���ڸ� ���ۿ� ä���.
	for (int i = 0; i < x_mul / 2; i++)
	{
		strcat_s(buf, 16, player);
	}

	// ���� ���̸�ŭ ����Ѵ�.
	for (int i = 0; i < y_mul; i++)
	{
		LHPutAttrCharEx(chr->x * x_mul, chr->y * y_mul + i, buf, bg, fg);
	}
}