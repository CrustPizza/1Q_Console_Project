#ifndef KEYCODE
#define KEYCODE

// �� ��ü�� ���
#define PLAYER "��"
#define OBJECT "��"
#define BOMB   "��"

// Ȯ�忡 ����ϴ� ũ�� ���
#define Y_Multiplier 2
#define X_Multiplier 4

#endif // !KEYCODE

void Draw_nPixel(int, int, const char*, const int, const int);
void DrawMap();
void DrawUI();

void GameInit();

bool Check_Move(int x, int y);