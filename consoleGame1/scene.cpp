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

// 현재 스크린 상태
enum screen_state Get_State()
{
	return scn_state;
}

void Set_State(screen_state state)
{
	scn_state = state;
}

// 타이틀 화면
bool Title_Screen()
{
	TitleDraw();

	if (check_bgm() == false)
	{
		PlayBGM(0);
	}

	// 컨트롤러에서 입력을 받는다 
	if (menu_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// 스테이지 고르는 화면으로 이동함.
			scn_state = stage;
			select_cursor = 0;
			break;
		case 1:
			// 게임 정보에 대한 화면
			scn_state = info;
			break;
		case 2:
			scn_state = option;
			select_cursor = 0;
			break;
		case 3:
			// 종료
			return false;
		}
	}

	// 범위를 벗어나지 않게 조절한다.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 3)
	{
		select_cursor = 3;
	}

	// 메뉴 그리기
	MenuDraw(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor);

	return true;
}

// 스테이지 화면
void Stage_Screen()
{
	// 컨트롤러에서 입력을 받는다 
	if (menu_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// 멀티 게임
			scn_state = multi;
			break;
		case 1:
			// 싱글 게임
			scn_state = single_game;
			break;
		case 2:
			// 타이틀로 돌아가기
			scn_state = title;
			select_cursor = 0;
			break;
		}
	}

	// 범위를 벗어나지 않게 조절한다.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 2)
	{
		select_cursor = 2;
	}

	// 멀티 모드, 싱글 모드를 선택한다.
	ModeListDraw(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor);
}

// 정보 화면
void Info_Screen()
{
	// 컨트롤러에서 입력을 받는다
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
			// 1P 게임패드
			set_button(&player[0], 0, true);
			flag = true;
			break;
		case 1:
			// 1P 키보드
			set_button(&player[0], 0, false);
			flag = true;
			break;
		case 2:
			// 2P 게임패드
			set_button(&player[1], 1, true);
			flag = true;
			break;
		case 3:
			// 2P 키보드
			set_button(&player[1], 1, false);
			flag = true;
			break;
		case 4:
			// 타이틀로 돌아가기
			scn_state = title;
			select_cursor = 0;
			break;
		}
	}

	// 범위를 벗어나지 않게 조절한다.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 4)
	{
		select_cursor = 4;
	}

	// 처리 완료 출력을 위한 Bool값
	flag = OptionDraw((cols - 59) / 2, MENU_LOCATION_Y, select_cursor, flag);
}

void Multi_Screen()
{
	if (menu_controller(&select_cursor, player) == true)
	{
		switch (select_cursor)
		{
		case 0:
			// 방 생성 선택
			scn_state = host_room;
			// 통신을 위한 호스트 소켓을 생성한다.
			if (host == nullptr)
			{
				host = make_shared<Server>(1);
			}
			break;
		case 1:
			// 방 참가 선택
			scn_state = user_room;
			// 통신을 위한 유저 소켓을 생성한다.
			if (user == nullptr)
			{
				user = make_shared<Client>();
			}
			break;
		case 2:
			// 뒤로가기
			scn_state = stage;
			select_cursor = 0;
			break;
		}
	}

	// 범위를 벗어나지 않게 조절한다.
	if (select_cursor < 0)
	{
		select_cursor = 0;
	}
	else if (select_cursor > 2)
	{
		select_cursor = 2;
	}

	// 방을 생성할 것인지 생성된 방에 참가할 것인지 결정한다.
	MultiDraw(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor);
}

/// <summary>
/// 네트워크에서 호스트 대기실
/// </summary>
void Host_Screen()
{
	HostRoom(MENU_LOCATION_X, MENU_LOCATION_Y, host);

	while (select_cursor == 1)
	{
		// 유저와 동기화를 위해 대기
		if (time(nullptr) - host->timer >= SYNC_SEC)
		{
			// 동기화가 끝나면 게임 화면으로 넘어간다.
			host->timer = GetTickCount64();
			scn_state = multi_game;
			return;
		}
	}

	if (host->get_client_count() != 0)
	{
		if (select_cursor == 0)
		{
			// 유저와 연결되었을 때 맵을 동기화 시켜준다.
			// 문자 0~2는 전송되는 바이트 크기
			// 문자 3은 전송 타입
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

			// 시간을 동일하게하여 동기화 시킨다.
			// 문자 3은 시간 타입
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
			// 뒤로가기
			scn_state = multi;
			host = nullptr;
		}

		select_cursor = 0;
	}
}

/// <summary>
/// 네트워크에서 유저 대기실
/// </summary>
void User_Screen()
{
	static string	input = "";
	static bool		is_error = false;

	// 유저 대기실
	// 커서와 입력 중인 IP 주소, IP 연결 성공시 출력하기 위한 Bool 변수를 전달한다.
	UserRoom(MENU_LOCATION_X, MENU_LOCATION_Y, select_cursor, input, user->gameStart, is_error);

	if (user->gameStart == true)
	{
		// 연결 성공시에 호스트와 동기화를 위해 대기
		if (time(nullptr) - user->timer >= SYNC_SEC)
		{
			// 동기화 성공후 timer 갱신하며 화면 변경
			user->timer = GetTickCount64();
			scn_state = multi_game;
			return;
		}
	}
	else if (number_controller(&select_cursor, player) == true)
	{
		is_error = false;
		// IP를 입력 받는다. ipv4 기준 크기를 넘지 않도록한다.
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
			// 호스트와 연결 시도
			if (user->Connect(Endpoint(input.c_str(), 4823)) == false)
			{
				is_error = true;
			}
			break;
		case 21:
			// 뒤로가기
			scn_state = multi;
			user = nullptr;
			input = "";
			break;
		}
	}
}

/// <summary>
/// 네트워크를 사용하여 플레이
/// </summary>
void Multi_Game()
{
	// 호스트인 경우
	if (host != nullptr)
	{
		// 키 입력 받기
		string temp = Multi_Player_Controller(&player[0]);

		if (temp.empty() == false)
		{
			// 문자 0~2 패킷 바이트 크기
			// 문자 3 동기화용 프레임 전달
			string buf = "    ";
			buf += temp;
			buf[0] = '0' + buf.length() / 100;
			buf[1] = '0' + (buf.length() % 100) / 10;
			buf[2] = '0' + (buf.length() % 10);
			buf[3] = '0' + ((host->frame + DO_FRAME) % 10);

			// 호스트의 Queue에 예약하고 클라이언트에게 전송한다.
			host->todo_list.push(std::make_pair(buf[3] - '0', temp));
			host->SendToAll(buf);
		}

		// 프레임을 맞추기 위한 타이머
		if (GetTickCount64() - host->timer >= 16)
		{
			host->frame++;
			host->timer = GetTickCount64();

			// Queue에 저장된 작업을 한다.
			while (host->todo_list.empty() == false)
			{
				switch ((Type)host->todo_list.front().second[0])
				{
				case Type::character_type:
					// 인자로 캐릭터 번호와 이동할 위치를 전달한다.
					character_move(&player[host->todo_list.front().second[1] - '0'],
						(Direction)(host->todo_list.front().second[2]));
					break;
				case Type::bomb_type:
					// 폭탄을 만든 캐릭터를 전달한다.
					create_bomb(&player[host->todo_list.front().second[1] - '0']);
					break;
				case Type::time_type:
					break;
				}
				host->todo_list.pop();
			}
		}
	}
	// 유저인 경우
	else if (user != nullptr)
	{
		// 키 입력 받기
		string temp = Multi_Player_Controller(&player[1]);

		if (temp.empty() == false)
		{
			// 문자 0~2 패킷 바이트 크기
			// 문자 3 동기화용 프레임 전달
			string buf = "    ";
			buf += temp;
			buf[0] = '0' + buf.length() / 100;
			buf[1] = '0' + (buf.length() % 100) / 10;
			buf[2] = '0' + (buf.length() % 10);
			buf[3] = '0' + ((user->frame + DO_FRAME) % 10);

			// 유저의 Queue에 예약하고 호스트에게 전송한다.
			user->todo_list.push(std::make_pair(buf[3] - '0', temp));

			// 호스트 송신 전용 Thread가 있으므로 buffer 사용시에 잠군다.
			std::lock_guard<recursive_mutex> lock(user->buffer_mutex);
			user->buffer = buf;
			user->send_on = true;
		}

		// 프레임을 맞추기 위한 타이머
		if (GetTickCount64() - user->timer >= 16)
		{
			user->frame++;
			user->timer = GetTickCount64();

			// Queue에 저장된 작업을 한다.
			while (user->todo_list.empty() == false)
			{
				switch ((Type)user->todo_list.front().second[0])
				{
				case Type::character_type:
					// 캐릭터와 이동할 위치를 전달한다.
					character_move(&player[user->todo_list.front().second[1] - '0'],
						(Direction)(user->todo_list.front().second[2]));
					break;
				case Type::bomb_type:
					// 폭탄을 만든 캐릭터를 전달한다.
					create_bomb(&player[user->todo_list.front().second[1] - '0']);
					break;
				case Type::time_type:
					break;
				}
				user->todo_list.pop();
			}
		}
	}

	// 맵과 UI 그리기
	DrawMap();
	DrawUI();

	// 폭탄이 일정시간 딜레이 후 폭발하며
	// 폭발에 의한 효과를 위한 업데이트 및 그리기
	Bomb_Update();

	// 캐릭터 그리기
	character_render(&player[0], X_Multiplier, Y_Multiplier, PLAYER, black, cyan);
	character_render(&player[1], X_Multiplier, Y_Multiplier, PLAYER, black, yellow);
}

/// <summary>
/// 네트워크를 사용하지 않고 플레이
/// </summary>
void Single_Game()
{
	// 1PC 게임에서 2명의 플레이어 입력 받기
	Player_Controller(&player[0]);
	Player_Controller(&player[1]);

	// 맵 그리기
	DrawMap();
	DrawUI();

	// 폭탄 업데이트
	Bomb_Update();

	// 캐릭터 그리기
	character_render(&player[0], X_Multiplier, Y_Multiplier, PLAYER, black, cyan);
	character_render(&player[1], X_Multiplier, Y_Multiplier, PLAYER, black, yellow);
}

/// <summary>
/// 엔딩, 결과창을 통해 누가 이겼는지 출력한다.
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