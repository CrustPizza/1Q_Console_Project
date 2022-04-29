#ifndef SAMPLE_H
#define SAMPLE_H
#include "sample.h"
#endif

// ����
void MaplistDraw_sample(const int x, const int y, const int select)
{
	LHPutAttrCharEx(40, 10, "[�ʼ���]", black, white);
	LHPutAttrCharEx(x, y, "Easy", black, white);
	LHPutAttrCharEx(x, y + 1, "Hard", black, white);
	LHPutAttrCharEx(x, y + 2, "Back", black, white);
	LHPutAttrCharEx(x - 2, y + select, ">", black, white);
}

// ����
void infoDraw_sample(const int x, const int y)
{
	LHPutAttrCharEx(20, 5, "GameInfo", black, white);

	LHPutAttrCharEx(x, y, "<2PLAYER>", black, white);
	LHPutAttrCharEx(x, y + 2, "��� ���� ���� �ı��ϸ� -10", black, white);
	LHPutAttrCharEx(x, y + 3, "������� ���� ���߸� -25", black, white);
	LHPutAttrCharEx(x, y + 4, "�� �� ����� �ı��ϸ� +5", black, white);
	LHPutAttrCharEx(x, y + 5, "�ϴû��� ��źLV UP", black, white);
					
	LHPutAttrCharEx(x, y + 7, "<1PLAYER>", black, white);
	LHPutAttrCharEx(x, y + 9, "�ڽ��� ���� �ı��ϸ� +7, ����� ����-5", black, white);
	LHPutAttrCharEx(x, y + 10, "���� ü���� �پ��� �ð��� ������ �������� Ŀ����.", black, white);
	LHPutAttrCharEx(x, y + 11, "���ھ�� �÷��̽ð��� �ı��� ����� ������ �������� �����ȴ�.", black, white);
}

// ����
void MenuDraw_sample(const int x, const int y, const int select)
{
	LHPutAttrCharEx(x, y, "Start", black, white);
	LHPutAttrCharEx(x, y + 1, "GameInfo", black, white);
	LHPutAttrCharEx(x, y + 2, "EndGame", black, white);
	LHPutAttrCharEx(x - 2, y + select, ">", black, white);
}

// ����
void titleDraw_sample()
{
	static int flag = 0;

	if (flag++ < 500)
	{
		LHPutAttrCharEx(40, 11, "Ÿ��Ʋ ���� ���� �׽�Ʈ", black, white);

		LHPutAttrCharEx(20, 10, "test", black, red);
	}
	else
	{
		if (flag > 1000)
		{
			flag = 0;
		}

		LHPutAttrCharEx(40, 11, "Ÿ��Ʋ ���� ���� �׽�Ʈ", white, black);

		LHPutAttrCharEx(20, 10, "test", blue, red);
	}
}