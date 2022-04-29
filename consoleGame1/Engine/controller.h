#define KEYBOARD_KEYS 256
#define PLAYERS 2

#ifndef KEY_STATE_H
#define KEY_STATE_H
enum Key_state
{
	UP,
	DOWN,
	STAY
};
#endif

enum Type
{
	map_type = '1',
	character_type,
	bomb_type,
	time_type
};

void key_init();
bool keyboard_check(const int key, const int state, char number);
bool gamepad_check(const int key, const int state, char number);

void set_button(Character* chr, int control_number, bool is_pad);
bool key_check(Character* chr, int key, int state);
bool key_check_multi(Character* chr, int key, int state);
bool menu_controller(int* select, Character* chr);
bool option_controller(int* select, Character* chr);
void Player_Controller(Character* chr);
string Multi_Player_Controller(Character* chr);
bool number_controller(int* select, Character* chr);