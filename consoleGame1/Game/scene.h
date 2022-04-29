#ifndef SCREEN_STATE_H
#define SCREEN_STATE_H
enum screen_state
{
	title,
	stage,
	info,
	option,
	multi,
	host_room,
	user_room,
	multi_game,
	single_game,
	ending
};
#endif

enum screen_state Get_State();
void Set_State(enum screen_state state);
bool Title_Screen();
void Stage_Screen();
void Info_Screen();
void Option_Screen();
void Multi_Screen();
void Host_Screen();
void User_Screen();
void Multi_Game();
void Single_Game();
void ending_screen(int win, int time);