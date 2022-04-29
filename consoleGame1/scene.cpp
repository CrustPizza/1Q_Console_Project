#include "stdafx.h"
#include "scene.h"

#define MENU_LOCATION_X 160
#define MENU_LOCATION_Y 30
#define SYNC_SEC		3
#define	DO_FRAME		2

static enum screen_state	scn_state = title;
static int					select_cursor = 0;
shared_ptr<Server>			host = nullptr;
shared_ptr<Client>			user = nullptr;

// ���� ��ũ�� ����
enum screen_state Get_State()
{
	return scn_state;
}

void Set_State(screen_state state)
{
	scn_state = state;
}

// Ÿ��Ʋ ȭ��
bool Title_Screen()
{
	TitleDraw();

	if (check_bgm() == false)
	{
		PlayBGM(0);
	}

	// ��Ʈ�ѷ����� �Է��� �޴´� 
	if (menu_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// �������� ���� ȭ������ �̵���.
			scn_state = stage;
			select_cursor = 0;
			break;
		case 1:
			// ���� ������ ���� ȭ��
			scn_state = info;
			break;
		case 2:
			scn_state = option;
			select_cursor = 0;
			break;
		case 3:
			// ����
			return false;
		}
	}

	// ������ ����� �ʰ� �����Ѵ�.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 3)
	{
		select_cursor = 3;
	}

	// �޴� �׸���
	MenuDraw(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor);

	return true;
}

// �������� ȭ��
void Stage_Screen()
{
	// ��Ʈ�ѷ����� �Է��� �޴´� 
	if (menu_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// ��Ƽ ����
			scn_state = multi;
			break;
		case 1:
			// �̱� ����
			scn_state = single_game;
			break;
		case 2:
			// Ÿ��Ʋ�� ���ư���
			scn_state = title;
			select_cursor = 0;
			break;
		}
	}

	// ������ ����� �ʰ� �����Ѵ�.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 2)
	{
		select_cursor = 2;
	}

	// ��Ƽ ���, �̱� ��带 �����Ѵ�.
	ModeListDraw(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor);
}

// ���� ȭ��
void Info_Screen()
{
	// ��Ʈ�ѷ����� �Է��� �޴´�
	if (menu_controller(&select_cursor, player) == true)
	{
		scn_state = title;
		select_cursor = 1;
	}

	InfoDraw(5, 10);
}

void Option_Screen()
{
	static bool flag = false;

	if (option_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// 1P �����е�
			set_button(&player[0], 0, true);
			flag = true;
			break;
		case 1:
			// 1P Ű����
			set_button(&player[0], 0, false);
			flag = true;
			break;
		case 2:
			// 2P �����е�
			set_button(&player[1], 1, true);
			flag = true;
			break;
		case 3:
			// 2P Ű����
			set_button(&player[1], 1, false);
			flag = true;
			break;
		case 4:
			// Ÿ��Ʋ�� ���ư���
			scn_state = title;
			select_cursor = 0;
			break;
		}
	}

	// ������ ����� �ʰ� �����Ѵ�.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 4)
	{
		select_cursor = 4;
	}

	// ó�� �Ϸ� ����� ���� Bool��
	flag = OptionDraw((cols - 59) / 2, MENU_LOCATION_Y, select_cursor, flag);
}

void Multi_Screen()
{
	if (menu_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// �� ���� ����
			scn_state = host_room;
			// ����� ���� ȣ��Ʈ ������ �����Ѵ�.
			if (host == nullptr)
			{
				host = make_shared<Server>(1);
			}
			break;
		case 1:
			// �� ���� ����
			scn_state = user_room;
			// ����� ���� ���� ������ �����Ѵ�.
			if (user == nullptr)
			{
				user = make_shared<Client>();
			}
			break;
		case 2:
			// �ڷΰ���
			scn_state = stage;
			select_cursor = 0;
			break;
		}
	}

	// ������ ����� �ʰ� �����Ѵ�.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 2)
	{
		select_cursor = 2;
	}

	// ���� ������ ������ ������ �濡 ������ ������ �����Ѵ�.
	MultiDraw(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor);
}

/// <summary>
/// ��Ʈ��ũ���� ȣ��Ʈ ����
/// </summary>
void Host_Screen()
{
	HostRoom(MENU_LOCATION_X, MENU_LOCATION_Y, host);

	while (select_cursor == 1)
	{
		// ������ ����ȭ�� ���� ���
		if (time(nullptr) - host->timer >= SYNC_SEC)
		{
			// ����ȭ�� ������ ���� ȭ������ �Ѿ��.
			host->timer = GetTickCount64();
			scn_state = multi_game;
			return;
		}
	}

	if (host->get_client_count() != 0)
	{
		if (select_cursor == 0)
		{
			// ������ ����Ǿ��� �� ���� ����ȭ �����ش�.
			// ���� 0~2�� ���۵Ǵ� ����Ʈ ũ��
			// ���� 3�� ���� Ÿ��
			string buf = "     ";
			for (int i = 0; i < map_h; i++)
			{
				for (int j = 0; j < map_w; j++)
				{
					buf += tempmap[i][j];
				}
			}
			buf[0] = '0' + (buf.length() / 100);
			buf[1] = '0' + (buf.length() % 100) / 10;
			buf[2] = '0' + (buf.length() % 10);
			buf[4] = (char)Type::map_type;

			host->SendToAll(buf);

			// �ð��� �����ϰ��Ͽ� ����ȭ ��Ų��.
			// ���� 3�� �ð� Ÿ��
			char cbuf[32] = {};
			host->timer = time(nullptr);
			_itoa_s(host->timer, cbuf, 32, 10);
			buf = "     ";
			buf += cbuf;
			buf[0] = '0' + (buf.length() / 100);
			buf[1] = '0' + (buf.length() % 100) / 10;
			buf[2] = '0' + (buf.length() % 10);
			buf[4] = (char)Type::time_type;

			host->SendToAll(buf);

			select_cursor = 1;
		}
	}
	else
	{
		if (menu_controller(&select_cursor, player) == true)
		{
			// �ڷΰ���
			scn_state = multi;
			host = nullptr;
		}

		select_cursor = 0;
	}
}

/// <summary>
/// ��Ʈ��ũ���� ���� ����
/// </summary>
void User_Screen()
{
	static string	input = "";
	static bool		is_error = false;

	// ���� ����
	// Ŀ���� �Է� ���� IP �ּ�, IP ���� ������ ����ϱ� ���� Bool ������ �����Ѵ�.
	UserRoom(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor, input, user->gameStart, is_error);

	if (user->gameStart == true)
	{
		// ���� �����ÿ� ȣ��Ʈ�� ����ȭ�� ���� ���
		if (time(nullptr) - user->timer >= SYNC_SEC)
		{
			// ����ȭ ������ timer �����ϸ� ȭ�� ����
			user->timer = GetTickCount64();
			scn_state = multi_game;
			return;
		}
	}
	else if (number_controller(&select_cursor, player) == true)
	{
		is_error = false;
		// IP�� �Է� �޴´�. ipv4 ���� ũ�⸦ ���� �ʵ����Ѵ�.
		if (input.length() < 15 && select_cursor <= 10)
		{
			if (select_cursor == 10)
			{
				input += '.';
			}
			else
			{
				input += '0' + select_cursor;
			}
		}

		switch (select_cursor)
		{
		case 11:
			// BACKSPACE
			if (input.empty() == false)
			{
				input.pop_back();
			}
			break;
		case 20:
			// ȣ��Ʈ�� ���� �õ�
			if (user->Connect(Endpoint(input.c_str(), 4823)) == false)
			{
				is_error = true;
			}
			break;
		case 21:
			// �ڷΰ���
			scn_state = multi;
			user = nullptr;
			input = "";
			break;
		}
	}
}

/// <summary>
/// ��Ʈ��ũ�� ����Ͽ� �÷���
/// </summary>
void Multi_Game()
{
	// ȣ��Ʈ�� ���
	if (host != nullptr)
	{
		// Ű �Է� �ޱ�
		string temp = Multi_Player_Controller(&player[0]);

		if (temp.empty() == false)
		{
			// ���� 0~2 ��Ŷ ����Ʈ ũ��
			// ���� 3 ����ȭ�� ������ ����
			string buf = "    ";
			buf += temp;
			buf[0] = '0' + buf.length() / 100;
			buf[1] = '0' + (buf.length() % 100) / 10;
			buf[2] = '0' + (buf.length() % 10);
			buf[3] = '0' + ((host->frame + DO_FRAME) % 10);

			// ȣ��Ʈ�� Queue�� �����ϰ� Ŭ���̾�Ʈ���� �����Ѵ�.
			host->todo_list.push(std::make_pair(buf[3] - '0', temp));
			host->SendToAll(buf);
		}

		// �������� ���߱� ���� Ÿ�̸�
		if (GetTickCount64() - host->timer >= 16)
		{
			host->frame++;
			host->timer = GetTickCount64();

			// Queue�� ����� �۾��� �Ѵ�.
			while (host->todo_list.empty() == false)
			{
				switch ((Type)host->todo_list.front().second[0])
				{
				case Type::character_type:
					// ���ڷ� ĳ���� ��ȣ�� �̵��� ��ġ�� �����Ѵ�.
					character_move(&player[host->todo_list.front().second[1] - '0'],
						(Direction)(host->todo_list.front().second[2]));
					break;
				case Type::bomb_type:
					// ��ź�� ���� ĳ���͸� �����Ѵ�.
					create_bomb(&player[host->todo_list.front().second[1] - '0']);
					break;
				case Type::time_type:
					break;
				}
				host->todo_list.pop();
			}
		}
	}
	// ������ ���
	else if (user != nullptr)
	{
		// Ű �Է� �ޱ�
		string temp = Multi_Player_Controller(&player[1]);

		if (temp.empty() == false)
		{
			// ���� 0~2 ��Ŷ ����Ʈ ũ��
			// ���� 3 ����ȭ�� ������ ����
			string buf = "    ";
			buf += temp;
			buf[0] = '0' + buf.length() / 100;
			buf[1] = '0' + (buf.length() % 100) / 10;
			buf[2] = '0' + (buf.length() % 10);
			buf[3] = '0' + ((user->frame + DO_FRAME) % 10);

			// ������ Queue�� �����ϰ� ȣ��Ʈ���� �����Ѵ�.
			user->todo_list.push(std::make_pair(buf[3] - '0', temp));

			// ȣ��Ʈ �۽� ���� Thread�� �����Ƿ� buffer ���ÿ� �ᱺ��.
			std::lock_guard<recursive_mutex> lock(user->buffer_mutex);
			user->buffer = buf;
			user->send_on = true;
		}

		// �������� ���߱� ���� Ÿ�̸�
		if (GetTickCount64() - user->timer >= 16)
		{
			user->frame++;
			user->timer = GetTickCount64();

			// Queue�� ����� �۾��� �Ѵ�.
			while (user->todo_list.empty() == false)
			{
				switch ((Type)user->todo_list.front().second[0])
				{
				case Type::character_type:
					// ĳ���Ϳ� �̵��� ��ġ�� �����Ѵ�.
					character_move(&player[user->todo_list.front().second[1] - '0'],
						(Direction)(user->todo_list.front().second[2]));
					break;
				case Type::bomb_type:
					// ��ź�� ���� ĳ���͸� �����Ѵ�.
					create_bomb(&player[user->todo_list.front().second[1] - '0']);
					break;
				case Type::time_type:
					break;
				}
				user->todo_list.pop();
			}
		}
	}

	// �ʰ� UI �׸���
	DrawMap();
	DrawUI();

	// ��ź�� �����ð� ������ �� �����ϸ�
	// ���߿� ���� ȿ���� ���� ������Ʈ �� �׸���
	Bomb_Update();

	// ĳ���� �׸���
	character_render(&player[0], X_Multiplier, Y_Multiplier, PLAYER, black, cyan);
	character_render(&player[1], X_Multiplier, Y_Multiplier, PLAYER, black, yellow);
}

/// <summary>
/// ��Ʈ��ũ�� ������� �ʰ� �÷���
/// </summary>
void Single_Game()
{
	// 1PC ���ӿ��� 2���� �÷��̾� �Է� �ޱ�
	Player_Controller(&player[0]);
	Player_Controller(&player[1]);

	// �� �׸���
	DrawMap();
	DrawUI();

	// ��ź ������Ʈ
	Bomb_Update();

	// ĳ���� �׸���
	character_render(&player[0], X_Multiplier, Y_Multiplier, PLAYER, black, cyan);
	character_render(&player[1], X_Multiplier, Y_Multiplier, PLAYER, black, yellow);
}

/// <summary>
/// ����, ���â�� ���� ���� �̰���� ����Ѵ�.
/// </summary>
void ending_screen(int win, int time)
{
	EndingDraw(win);
	if (menu_controller(&select_cursor, &player[0]) == true && GetTickCount64() - time >= 2000)
	{
		scn_state = title;
		GameInit();
		character_init(&player[0], 2, 2, 0, player[0].ctl.gamepad, 0);
		character_init(&player[1], map_w - 3, map_h - 3, 1, player[1].ctl.gamepad, 1);
		select_cursor = 0;
	}
}