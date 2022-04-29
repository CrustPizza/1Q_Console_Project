#include "stdafx.h"

/// <summary>
/// 그릴 때 확장된 크기만큼 확대해서 그리는 함수
/// </summary>
void Draw_nPixel(int x, int y, const char* pStr, const int bg, const int fg)
{
	char buf[16] = "";

	// 가로 크기만큼 버퍼에 저장
	for (int i = 0; i < X_Multiplier / 2; i++)
	{
		strcat_s(buf, 16, pStr);
	}

	// 세로 크기만큼 출력
	for (int i = 0; i < Y_Multiplier; i++)
	{
		LHPutAttrCharEx(x, y + i, buf, bg, fg);
	}
}

/// <summary>
/// 게임 관련 초기화 함수
/// </summary>
void GameInit()
{
	Bomb_Init();
	map_init();
}

/// <summary>
/// 게임 화면에서 맵을 그림
/// </summary>
void DrawMap()
{
	for (int h = 0; h < map_h; h++)
	{
		// 출력할 텍스트 문자열 버퍼
		char text[Y_Multiplier][256] = { "", "" };

		// 출력할 컬러 값 버퍼
		WORD color[Y_Multiplier][256] = { 0, };

		// 확장된 크기(X_Mul, Y_Mul)만큼 버퍼에 담아서 한줄씩 출력시킨다.
		for (int w = 0; w < map_w; w++)
		{
			switch (tempmap[h][w])
			{
				// 맵 테두리의 흰색 벽
			case '1':
				for (int y = 0; y < Y_Multiplier; y++)
				{
					for (int x = 0; x < X_Multiplier / 2; x++)
					{
						strcat_s(text[y], 256, OBJECT);
						color[y][w * X_Multiplier + x * 2] = black * 16 + white;
						color[y][w * X_Multiplier + x * 2 + 1] = black * 16 + white;
					}
				}
				break;
				// 파괴 불가능한 회색 블럭
			case '4':
				for (int y = 0; y < Y_Multiplier; y++)
				{
					for (int x = 0; x < X_Multiplier / 2; x++)
					{
						strcat_s(text[y], 256, OBJECT);
						color[y][w * X_Multiplier + x * 2] = black * 16 + darkgray;
						color[y][w * X_Multiplier + x * 2 + 1] = black * 16 + darkgray;
					}
				}
				break;
				// 플레이어 1의 파란 블럭
			case 'A': case 'B':	case 'C': case 'D':
				for (int y = 0; y < Y_Multiplier; y++)
				{
					for (int x = 0; x < X_Multiplier / 2; x++)
					{
						strcat_s(text[y], 256, OBJECT);
						color[y][w * X_Multiplier + x * 2] = black * 16 + lightblue;
						color[y][w * X_Multiplier + x * 2 + 1] = black * 16 + lightblue;
					}
				}
				break;
				// 플레이어 2의 노란 블럭
			case 'W': case 'X':	case 'Y': case 'Z':
				for (int y = 0; y < Y_Multiplier; y++)
				{
					for (int x = 0; x < X_Multiplier / 2; x++)
					{
						strcat_s(text[y], 256, OBJECT);
						color[y][w * X_Multiplier + x * 2] = black * 16 + yellow;
						color[y][w * X_Multiplier + x * 2 + 1] = black * 16 + yellow;
					}
				}
				break;
				// 공백
			default:
				for (int y = 0; y < Y_Multiplier; y++)
				{
					for (int x = 0; x < X_Multiplier / 2; x++)
					{
						strcat_s(text[y], 256, "  ");
						color[y][w * X_Multiplier + x * 2] = black;
						color[y][w * X_Multiplier + x * 2 + 1] = black;
					}
				}
				break;
			}
		}

		for (int i = 0; i < Y_Multiplier; i++)
		{
			LHPutAttrCharExLine(0, h * 2 + i, text[i], color[i]);
		}
	}
}

/// <summary>
/// 게임 화면에서 플레이어 정보가 적힌 UI
/// </summary>
void DrawUI()
{
	// 분할 UI
	char midline[] = "                         ";
	LHPutAttrCharEx(160, 0, midline, darkgray, white);
	LHPutAttrCharEx(160, 19, midline, darkgray, white);
	LHPutAttrCharEx(160, 20, midline, darkgray, white);
	LHPutAttrCharEx(160, 39, midline, darkgray, white);

	for (int i = 0; i < PLAYERS; i++)
	{
		char cbuf[64];
		// 플레이어 체력
		sprintf_s(cbuf, 64, "PLAYER %d HP : %d", i + 1, player[i].health);
		LHPutAttrCharEx(163, i * 20 + 3, cbuf, black, lightcyan + 3 * i);

		WORD color[20] = {};

		for (int j = 0; j < 20; j++)
		{
			if ((j + 1) * 5 < player[i].health)
			{
				color[j] = green;
			}
			else
			{
				color[j] = lightgray;
			}
		}

		LHPutAttrCharExLine(162, i * 20 + 5, "■■■■■■■■■■", color);

		// 플레이어 폭탄 레벨
		sprintf_s(cbuf, 64, "BOMB LEVEL     : %d", player[i].bomb_level);
		LHPutAttrCharEx(163, i * 20 + 8, cbuf, black, white);
		memset(cbuf, 0, 64);
		for (int j = 0; j < player[i].bomb_level; j++)
		{
			strcat_s(cbuf, 64, "■");
		}
		LHPutAttrCharEx(162, i * 20 + 10, cbuf, black, red);

		// 플레이어 최대 폭탄 갯수
		sprintf_s(cbuf, 64, "BOMB MAX COUNT : %d", player[i].bomb_max_count);
		LHPutAttrCharEx(163, i * 20 + 13, cbuf, black, white);
		memset(cbuf, 0, 64);
		for (int j = 0; j < player[i].bomb_max_count; j++)
		{
			strcat_s(cbuf, 64, "■");
		}
		LHPutAttrCharEx(162, i * 20 + 15, cbuf, black, brown);
	}
}

/// <summary>
/// 캐릭터가 이동 가능한지 확인하는 함수
/// </summary>
bool Check_Move(int x, int y)
{
	// 맵에서 비어있는지 확인
	if (tempmap[y][x] != '0')
	{
		return false;
	}

	// 캐릭터가 있는지 확인
	for (int i = 0; i < PLAYERS; i++)
	{
		if (player[i].x == x && player[i].y == y)
		{
			return false;
		}
	}

	return true;
}