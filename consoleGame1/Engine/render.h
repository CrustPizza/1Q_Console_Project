#ifndef COLORLIST
#define COLORLIST

enum {
	black,
	blue,
	green,
	cyan,
	red,
	purple,
	brown,
	lightgray,
	darkgray,
	lightblue,
	lightgreen,
	lightcyan,
	lightred,
	lightpurple,
	yellow,
	white
};
#endif // !CORORLIST

// Ã¢ Å©±â
extern const int cols;
extern const int lines;

void screen_init();
void screen_release();
void LHPutAttrCharEx(const int x, const int y, const char* pStr, const WORD bg, const WORD fg);
void LHPutAttrCharExLine(const int x, const int y, const char* pStr, WORD* color);
void render();