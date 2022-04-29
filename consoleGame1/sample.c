#ifndef SAMPLE_H
#define SAMPLE_H
#include "sample.h"
#endif

// 샘플
void MaplistDraw_sample(const int x, const int y, const int select)
{
	LHPutAttrCharEx(40, 10, "[맵선택]", black, white);
	LHPutAttrCharEx(x, y, "Easy", black, white);
	LHPutAttrCharEx(x, y + 1, "Hard", black, white);
	LHPutAttrCharEx(x, y + 2, "Back", black, white);
	LHPutAttrCharEx(x - 2, y + select, ">", black, white);
}

// 샘플
void infoDraw_sample(const int x, const int y)
{
	LHPutAttrCharEx(20, 5, "GameInfo", black, white);

	LHPutAttrCharEx(x, y, "<2PLAYER>", black, white);
	LHPutAttrCharEx(x, y + 2, "상대 색의 블럭을 파괴하면 -10", black, white);
	LHPutAttrCharEx(x, y + 3, "상대편을 직접 맞추면 -25", black, white);
	LHPutAttrCharEx(x, y + 4, "내 색 블록을 파괴하면 +5", black, white);
	LHPutAttrCharEx(x, y + 5, "하늘색은 폭탄LV UP", black, white);
					
	LHPutAttrCharEx(x, y + 7, "<1PLAYER>", black, white);
	LHPutAttrCharEx(x, y + 9, "자신의 색을 파괴하면 +7, 상대의 색은-5", black, white);
	LHPutAttrCharEx(x, y + 10, "매초 체력이 줄어들며 시간이 갈수록 증가폭이 커진다.", black, white);
	LHPutAttrCharEx(x, y + 11, "스코어는 플레이시간과 파괴한 블록의 종류와 수에의해 결정된다.", black, white);
}

// 샘플
void MenuDraw_sample(const int x, const int y, const int select)
{
	LHPutAttrCharEx(x, y, "Start", black, white);
	LHPutAttrCharEx(x, y + 1, "GameInfo", black, white);
	LHPutAttrCharEx(x, y + 2, "EndGame", black, white);
	LHPutAttrCharEx(x - 2, y + select, ">", black, white);
}

// 샘플
void titleDraw_sample()
{
	static int flag = 0;

	if (flag++ < 500)
	{
		LHPutAttrCharEx(40, 11, "타이틀 적을 공간 테스트", black, white);

		LHPutAttrCharEx(20, 10, "test", black, red);
	}
	else
	{
		if (flag > 1000)
		{
			flag = 0;
		}

		LHPutAttrCharEx(40, 11, "타이틀 적을 공간 테스트", white, black);

		LHPutAttrCharEx(20, 10, "test", blue, red);
	}
}