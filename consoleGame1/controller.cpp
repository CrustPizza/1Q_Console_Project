#include "stdafx.h"
#include <xinput.h>
#pragma comment(lib, "xinput.lib")

// 키를 누르고 땠을 때 조작을 위한 Bool 배열
bool keyboard_down[PLAYERS][KEYBOARD_KEYS];
bool keyboard_up[PLAYERS][KEYBOARD_KEYS];

// 게임패드를 누르고 땠을 때 조작을 위한 Bitmask
WORD gamepad_down[PLAYERS];
WORD gamepad_up[PLAYERS];

// 초기화
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
		// 버튼을 눌렀다가 땐 타이밍
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
		// 버튼을 누른 타이밍
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
		// 버튼을 누른채로 유지
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
		// 버튼을 눌렀다가 땐 타이밍
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
		// 버튼을 누른 타이밍
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
		// 버튼을 누른채로 유지
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
		// 버튼을 눌렀다가 땐 타이밍
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
		// 버튼을 누른 타이밍
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
		// 버튼을 누른채로 유지
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
		// 패드일 경우 아래의 키 값을 입력시킴
		chr->ctl.gamepad = true;

		chr->ctl.up = XINPUT_GAMEPAD_DPAD_UP;
		chr->ctl.down = XINPUT_GAMEPAD_DPAD_DOWN;
		chr->ctl.left = XINPUT_GAMEPAD_DPAD_LEFT;
		chr->ctl.right = XINPUT_GAMEPAD_DPAD_RIGHT;
		chr->ctl.select = XINPUT_GAMEPAD_A;
	}
	else
	{
		// 키보드일 경우 아래의 키 값을 입력시킴
		switch (control_number)
		{
		case 0:
			// 1P용
			chr->ctl.up = 'W';
			chr->ctl.down = 'S';
			chr->ctl.left = 'A';
			chr->ctl.right = 'D';
			chr->ctl.select = VK_SPACE;
			break;
		case 1:
			// 2P용
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

// 게임 패드와 키보드의 통합 컨트롤러
bool key_check(Character* chr, int key, int state)
{
	if (chr->ctl.gamepad == true)
	{
		// 패드일 경우 패드 결과 리턴
		return gamepad_check(key, state, chr->number);
	}

	// 성능 향상을 위한 조치
	if (_kbhit() == 0)
	{
		return false;
	}

	// 키보드일 경우 키보드 결과 리턴
	return keyboard_check(key, state, chr->number);
}

// 게임 패드와 키보드의 통합 컨트롤러
bool key_check_multi(Character* chr, int key, int state)
{
	if (chr->ctl.gamepad == true)
	{
		gamepad_check_multi(key, state, chr->number);
	}

	// 성능 향상을 위한 조치
	if (_kbhit() == 0)
	{
		return false;
	}

	// 키보드일 경우 키보드 결과 리턴
	return keyboard_check(key, state, 0);
}

bool option_controller(int* select, Character* chr)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		// 왼쪽
		if (key_check(&chr[i], chr[i].ctl.left, DOWN) == true)
		{
			(*select)--;
		}

		// 오른쪽
		if (key_check(&chr[i], chr[i].ctl.right, DOWN) == true)
		{
			(*select)++;
		}

		// 확인
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
		// 위로
		if (key_check(&chr[i], chr[i].ctl.up, DOWN) == true)
		{
			(*select)--;
		}

		// 아래로
		if (key_check(&chr[i], chr[i].ctl.down, DOWN) == true)
		{
			(*select)++;
		}

		// 확인
		if (key_check(&chr[i], chr[i].ctl.select, DOWN) == true)
		{
			return true;
		}
	}

	return false;
}

// 플레이어 컨트롤러
void Player_Controller(Character* chr)
{
	// 위로
	if (key_check(chr, chr->ctl.up, DOWN) == true)
	{
		// 이동 가능 여부 확인
		if (Check_Move(chr->x, chr->y - 1) == true)
		{
			chr->y--;
		}
	}

	// 아래로
	if (key_check(chr, chr->ctl.down, DOWN) == true)
	{
		// 이동 가능 여부 확인
		if (Check_Move(chr->x, chr->y + 1) == true)
		{
			chr->y++;
		}
	}

	// 왼쪽으로
	if (key_check(chr, chr->ctl.left, DOWN) == true)
	{
		// 이동 가능 여부 확인
		if (Check_Move(chr->x - 1, chr->y) == true)
		{
			chr->x--;
		}
	}

	// 오른쪽으로
	if (key_check(chr, chr->ctl.right, DOWN) == true)
	{
		// 이동 가능 여부 확인
		if (Check_Move(chr->x + 1, chr->y) == true)
		{
			chr->x++;
		}
	}

	// 확인
	if (key_check(chr, chr->ctl.select, DOWN) == true)
	{
		// 폭탄 작업
		create_bomb(chr);
	}
}

string Multi_Player_Controller(Character* chr)
{
	string ret = "";

	// 위로
	if (key_check_multi(chr, chr->ctl.up, DOWN) == true)
	{
		// 패킷 전송용 타입지정
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::UP;
	}

	// 아래로
	if (key_check_multi(chr, chr->ctl.down, DOWN) == true)
	{
		// 패킷 전송용 타입지정
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::DOWN;
	}

	// 왼쪽으로
	if (key_check_multi(chr, chr->ctl.left, DOWN) == true)
	{
		// 패킷 전송용 타입지정
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::LEFT;
	}

	// 오른쪽으로
	if (key_check_multi(chr, chr->ctl.right, DOWN) == true)
	{
		// 패킷 전송용 타입지정
		ret = (char)Type::character_type;
		ret += '0' + chr->number;
		ret += (char)Direction::RIGHT;
	}

	// 확인
	if (key_check_multi(chr, chr->ctl.select, DOWN) == true)
	{
		// 패킷 전송용 타입지정
		ret = (char)Type::bomb_type;
		ret += '0' + chr->number;
	}

	return ret;
}

bool number_controller(int* select, Character* chr)
{
	for (int i = 0; i < PLAYERS; i++)
	{
		// 왼쪽으로
		if (key_check(&chr[i], chr[i].ctl.left, DOWN) == true)
		{
			if (0 < *select)
			{
				(*select)--;
			}
		}

		// 오른쪽으로
		if (key_check(&chr[i], chr[i].ctl.right, DOWN) == true)
		{
			if (*select < 11)
			{
				(*select)++;
			}
		}

		// 위로
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

		// 아래로
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

		// 확인
		if (key_check(&chr[i], chr[i].ctl.select, DOWN) == true)
		{
			return true;
		}
	}

	// 키보드 동작을 위한 것으로 키보드 입력이 없을시에 밑을 확인하지 않는다.
	if (_kbhit() == 0)
	{
		return false;
	}

	// 0부터 9까지 입력을 확인한다.
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