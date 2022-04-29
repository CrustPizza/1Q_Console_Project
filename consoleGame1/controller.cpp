#include "stdafx.h"
#include <xinput.h>
#pragma comment(lib, "xinput.lib")

// Ű�� ������ ���� �� ������ ���� Bool �迭
bool keyboard_down[PLAYERS][KEYBOARD_KEYS];
bool keyboard_up[PLAYERS][KEYBOARD_KEYS];

// �����е带 ������ ���� �� ������ ���� Bitmask
WORD gamepad_down[PLAYERS];
WORD gamepad_up[PLAYERS];

// �ʱ�ȭ
void key_init()
{
	for (int j = 0; j < PLAYERS; j++)
	{
		for (int i = 0; i < KEYBOARD_KEYS; i++)
		{
			keyboard_down[j][i] = false;
			keyboard_up[j][i] = false;
		}
	}

	for (int i = 0; i < PLAYERS; i++)
	{
		gamepad_down[i] = 0;
		gamepad_up[i] = 0;
	}
}

bool keyboard_check(const int key, const int state, char number)
{
	switch (state)
	{
		// ��ư�� �����ٰ� �� Ÿ�̹�
	case UP:
		if ((GetAsyncKeyState(key) & 0x8000) != 0)
		{
			keyboard_up[number][key] = true;
		}
		else if (keyboard_up[number][key] == true)
		{
			keyboard_up[number][key] = false;
			return true;
		}

		return false;
		// ��ư�� ���� Ÿ�̹�
	case DOWN:
		if ((GetAsyncKeyState(key) & 0x8000) != 0)
		{
			if (false == keyboard_down[number][key])
			{
				keyboard_down[number][key] = true;
				return true;
			}
		}
		else
		{
			keyboard_down[number][key] = false;
		}

		return false;
		// ��ư�� ����ä�� ����
	case STAY:
		if ((GetAsyncKeyState(key) & 0x8000) != 0)
		{
			return true;
		}

		return false;
	}

	return false;
}

bool gamepad_check_multi(const int key, const int state, char number)
{
	XINPUT_STATE x_state;
	ZeroMemory(&x_state, sizeof(XINPUT_STATE));

	XInputGetState(0, &x_state);

	switch (state)
	{
		// ��ư�� �����ٰ� �� Ÿ�̹�
	case UP:
		if ((x_state.Gamepad.wButtons & key) != 0)
		{
			gamepad_up[number] |= key;
		}
		else if ((gamepad_up[number] & key) != 0)
		{
			gamepad_up[number] &= ~key;
			return true;
		}

		return false;
		// ��ư�� ���� Ÿ�̹�
	case DOWN:
		if ((x_state.Gamepad.wButtons & key) != 0)
		{
			if ((gamepad_down[number] & key) == 0)
			{
				gamepad_down[number] |= key;
				return true;
			}
		}
		else if ((gamepad_down[number] & key) != 0)
		{
			gamepad_down[number] &= ~key;
		}

		return false;
		// ��ư�� ����ä�� ����
	case STAY:
		if ((x_state.Gamepad.wButtons & key) != 0)
		{
			return true;
		}

		return false;
	}

	return false;
}

bool gamepad_check(const int key, const int state, char number)
{
	XINPUT_STATE x_state;
	ZeroMemory(&x_state, sizeof(XINPUT_STATE));

	XInputGetState(number, &x_state);

	switch (state)
	{
		// ��ư�� �����ٰ� �� Ÿ�̹�
	case UP:
		if ((x_state.Gamepad.wButtons & key) != 0)
		{
			gamepad_up[number] |= key;
		}
		else if ((gamepad_up[number] & key) != 0)
		{
			gamepad_up[number] &= ~key;
			return true;
		}

		return false;
		// ��ư�� ���� Ÿ�̹�
	case DOWN:
		if ((x_state.Gamepad.wButtons & key) != 0)
		{
			if ((gamepad_down[number] & key) == 0)
			{
				gamepad_down[number] |= key;
				return true;
			}
		}
		else if ((gamepad_down[number] & key) != 0)
		{
			gamepad_down[number] &= ~key;
		}

		return false;
		// ��ư�� ����ä�� ����
	case STAY:
		if ((x_state.Gamepad.wButtons & key) != 0)
		{
			return true;
		}

		return false;
	}

	return false;
}

void set_button(Character* chr, int control_number, bool is_pad)
{
	if (is_pad == true)
	{
		// �е��� ��� �Ʒ��� Ű ���� �Է½�Ŵ
		chr->ctl.gamepad = true;

		chr->ctl.up = XINPUT_GAMEPAD_DPAD_UP;
		chr->ctl.down = XINPUT_GAMEPAD_DPAD_DOWN;
		chr->ctl.left = XINPUT_GAMEPAD_DPAD_LEFT;
		chr->ctl.right = XINPUT_GAMEPAD_DPAD_RIGHT;
		chr->ctl.select = XINPUT_GAMEPAD_A;
	}
	else
	{
		// Ű������ ��� �Ʒ��� Ű ���� �Է½�Ŵ
		switch (control_number)
		{
		case 0:
			// 1P��
			chr->ctl.up = 'W';
			chr->ctl.down = 'S';
			chr->ctl.left = 'A';
			chr->ctl.right = 'D';
			chr->ctl.select = VK_SPACE;
			break;
		case 1:
			// 2P��
			chr->ctl.up = VK_UP;
			chr->ctl.down = VK_DOWN;
			chr->ctl.left = VK_LEFT;
			chr->ctl.right = VK_RIGHT;
			chr->ctl.select = VK_RETURN;
			break;
		}

		chr->ctl.gamepad = false;
	}
}

// ���� �е�� Ű������ ���� ��Ʈ�ѷ�
bool key_check(Character* chr, int key, int state)
{
	if (chr->ctl.gamepad == true)
	{
		// �е��� ��� �е� ��� ����
		return gamepad_check(key, state, chr->number);
	}

	// ���� ����� ���� ��ġ
	if (_kbhit() == 0)
	{
		return false;
	}

	// Ű������ ��� Ű���� ��� ����
	return keyboard_check(key, state, chr->number);
}

// ���� �е�� Ű������ ���� ��Ʈ�ѷ�
bool key_check_multi(Character* chr, int key, int state)
{
	if (chr->ctl.gamepad == true)
	{
		gamepad_check_multi(key, state, chr->number);
	}

	// ���� ����� ���� ��ġ
	if (_kbhit() == 0)
	{
		return false;
	}

	// Ű������ ��� Ű���� ��� ����
	return keyboard_check(key, state, 0);
}

bool option_controller(int* select, Character* chr)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		// ����
		if (key_check(&chr[i], chr[i].ctl.left, DOWN) == true)
		{
			(*select)--;
		}

		// ������
		if (key_check(&chr[i], chr[i].ctl.right, DOWN) == true)
		{
			(*select)++;
		}

		// Ȯ��
		if (key_check(&chr[i], chr[i].ctl.select, DOWN) == true)
		{
			return true;
		}
	}

	return false;
}

bool menu_controller(int* select, Character* chr)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		// ����
		if (key_check(&chr[i], chr[i].ctl.up, DOWN) == true)
		{
			(*select)--;
		}

		// �Ʒ���
		if (key_check(&chr[i], chr[i].ctl.down, DOWN) == true)
		{
			(*select)++;
		}

		// Ȯ��
		if (key_check(&chr[i], chr[i].ctl.select, DOWN) == true)
		{
			return true;
		}
	}

	return false;
}

// �÷��̾� ��Ʈ�ѷ�
void Player_Controller(Character* chr)
{
	// ����
	if (key_check(chr, chr->ctl.up, DOWN) == true)
	{
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x, chr->y - 1) == true)
		{
			chr->y--;
		}
	}

	// �Ʒ���
	if (key_check(chr, chr->ctl.down, DOWN) == true)
	{
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x, chr->y + 1) == true)
		{
			chr->y++;
		}
	}

	// ��������
	if (key_check(chr, chr->ctl.left, DOWN) == true)
	{
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x - 1, chr->y) == true)
		{
			chr->x--;
		}
	}

	// ����������
	if (key_check(chr, chr->ctl.right, DOWN) == true)
	{
		// �̵� ���� ���� Ȯ��
		if (Check_Move(chr->x + 1, chr->y) == true)
		{
			chr->x++;
		}
	}

	// Ȯ��
	if (key_check(chr, chr->ctl.select, DOWN) == true)
	{
		// ��ź �۾�
		create_bomb(chr);
	}
}

string Multi_Player_Controller(Character* chr)
{
	string ret = "";

	// ����
	if (key_check_multi(chr, chr->ctl.up, DOWN) == true)
	{
		// ��Ŷ ���ۿ� Ÿ������
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::UP;
	}

	// �Ʒ���
	if (key_check_multi(chr, chr->ctl.down, DOWN) == true)
	{
		// ��Ŷ ���ۿ� Ÿ������
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::DOWN;
	}

	// ��������
	if (key_check_multi(chr, chr->ctl.left, DOWN) == true)
	{
		// ��Ŷ ���ۿ� Ÿ������
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::LEFT;
	}

	// ����������
	if (key_check_multi(chr, chr->ctl.right, DOWN) == true)
	{
		// ��Ŷ ���ۿ� Ÿ������
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::RIGHT;
	}

	// Ȯ��
	if (key_check_multi(chr, chr->ctl.select, DOWN) == true)
	{
		// ��Ŷ ���ۿ� Ÿ������
		ret = (char)Type::bomb_type;
		ret += '0' + chr->number;
	}

	return ret;
}

bool number_controller(int* select, Character* chr)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		// ��������
		if (key_check(&chr[i], chr[i].ctl.left, DOWN) == true)
		{
			if (0 < *select)
			{
				(*select)--;
			}
		}

		// ����������
		if (key_check(&chr[i], chr[i].ctl.right, DOWN) == true)
		{
			if (*select < 11)
			{
				(*select)++;
			}
		}

		// ����
		if (key_check(&chr[i], chr[i].ctl.up, DOWN) == true)
		{
			if (*select > 15)
			{
				*select = 0;
			}
			else if (*select > 20)
			{
				*select = 20;
			}
		}

		// �Ʒ���
		if (key_check(&chr[i], chr[i].ctl.down, DOWN) == true)
		{
			if (*select < 20)
			{
				*select = 20;
			}
			else if (*select == 20)
			{
				*select = 21;
			}
		}

		// Ȯ��
		if (key_check(&chr[i], chr[i].ctl.select, DOWN) == true)
		{
			return true;
		}
	}

	// Ű���� ������ ���� ������ Ű���� �Է��� �����ÿ� ���� Ȯ������ �ʴ´�.
	if (_kbhit() == 0)
	{
		return false;
	}

	// 0���� 9���� �Է��� Ȯ���Ѵ�.
	for (int i = 0; i < 10; i++)
	{
		if (keyboard_check('0' + i, DOWN, chr->number) == true)
		{
			*select = i;
			return true;
		}

		if (keyboard_check(VK_NUMPAD0 + i, DOWN, chr->number) == true)
		{
			*select = i;
			return true;
		}
	}

	// Dot .
	if (keyboard_check(VK_OEM_PERIOD, DOWN, chr->number) == true)
	{
		*select = 10;
		return true;
	}

	// Backspace <-
	if (keyboard_check(VK_BACK, DOWN, chr->number) == true)
	{
		*select = 11;
		return true;
	}

	return false;
}