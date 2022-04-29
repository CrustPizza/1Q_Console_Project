#ifndef KEYCODE
#define KEYCODE

// 각 개체의 모양
#define PLAYER "■"
#define OBJECT "▣"
#define BOMB   "◈"

// 확장에 사용하는 크기 상수
#define Y_Multiplier 2
#define X_Multiplier 4

#endif // !KEYCODE

void Draw_nPixel(int, int, const char*, const int, const int);
void DrawMap();
void DrawUI();

void GameInit();

bool Check_Move(int x, int y);