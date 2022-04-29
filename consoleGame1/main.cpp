#include "stdafx.h"

static bool	turn_on = true;
Character	player[2];
int			winner = 2;

int main()
{
	int ending_timer;

	// function
	srand(time(NULL));

	// 초기화 리스트
	key_init();
	screen_init();
	SoundSystem();
	SoundSystem2();
	GameInit();
	character_init(&player[0], 2, 2, 0, false, 0);
	character_init(&player[1], map_w - 3, map_h - 3, 1, false, 1);

	while (turn_on == true)
	{
		// 상태에 따라 다른 화면 출력
		switch (Get_State())
		{
		case title:
			turn_on = Title_Screen();
			cursor_changer();
			break;
		case stage:
			Stage_Screen();
			cursor_changer();
			break;
		case option:
			Option_Screen();
			cursor_changer();
			break;
		case info:
			Info_Screen();
			break;
		case multi:
			if (check_bgm() == true)
			{
				StopBGM(0);
			}
			Multi_Screen();
			cursor_changer();
			break;
		case host_room:
			Host_Screen();
			cursor_changer();
			break;
		case user_room:
			User_Screen();
			cursor_changer();
			break;
		case multi_game:
			if (check_bgm() == true)
			{
				StopBGM(0);
			}
			Multi_Game();

			if (check_bgm_ingame() == false)
			{
				PlayBGM(1);
			}
			if (player[0].health <= 0 || player[1].health <= 0)
			{
				winner = player[0].health <= 0 ? 1 : 0;
				Set_State(ending);
				PlayEFFECT(APPLAUSE);
			}
			break;
		case single_game:
			if (check_bgm() == true)
			{
				StopBGM(0);
			}
			Single_Game();
			if (check_bgm_ingame() == false)
			{
				PlayBGM(1);
			}

			if (player[0].health <= 0 || player[1].health <= 0)
			{
				winner = player[0].health <= 0 ? 1 : 0;
				Set_State(ending);
				PlayEFFECT(APPLAUSE);
			}
			break;
		case ending:
			if (check_bgm_ingame() == true)
			{
				StopBGM(1);
				ending_timer = GetTickCount64();
			}
			ending_screen(winner, ending_timer);
			break;
		}

		render();
	}

	screen_release();

	return 0;
}