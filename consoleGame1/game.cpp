#include "stdafx.h"

/// <summary>
/// �׸� �� Ȯ��� ũ�⸸ŭ Ȯ���ؼ� �׸��� �Լ�
/// </summary>
void Draw_nPixel(int x, int y, const char* pStr, const int bg, const int fg)
{
	char buf[16] = "";

	// ���� ũ�⸸ŭ ���ۿ� ����
	for (int i = 0; i < X_Multiplier / 2; i++)
	{
		strcat_s(buf, 16, pStr);
	}

	// ���� ũ�⸸ŭ ���
	for (int i = 0; i < Y_Multiplier; i++)
	{
		LHPutAttrCharEx(x, y + i, buf, bg, fg);
	}
}

/// <summary>
/// ���� ���� �ʱ�ȭ �Լ�
/// </summary>
void GameInit()
{
	Bomb_Init();
	map_init();
}

/// <summary>
/// ���� ȭ�鿡�� ���� �׸�
/// </summary>
void DrawMap()
{
	for (int h = 0; h < map_h; h++)
	{
		// ����� �ؽ�Ʈ ���ڿ� ����
		char text[Y_Multiplier][256] = { "", "" };

		// ����� �÷� �� ����
		WORD color[Y_Multiplier][256] = { 0, };

		// Ȯ��� ũ��(X_Mul, Y_Mul)��ŭ ���ۿ� ��Ƽ� ���پ� ��½�Ų��.
		for (int w = 0; w < map_w; w++)
		{
			switch (tempmap[h][w])
			{
				// �� �׵θ��� ��� ��
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
				// �ı� �Ұ����� ȸ�� ��
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
				// �÷��̾� 1�� �Ķ� ��
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
				// �÷��̾� 2�� ��� ��
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
				// ����
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
/// ���� ȭ�鿡�� �÷��̾� ������ ���� UI
/// </summary>
void DrawUI()
{
	// ���� UI
	char midline[] = "                         ";
	LHPutAttrCharEx(160, 0, midline, darkgray, white);
	LHPutAttrCharEx(160, 19, midline, darkgray, white);
	LHPutAttrCharEx(160, 20, midline, darkgray, white);
	LHPutAttrCharEx(160, 39, midline, darkgray, white);

	for (int i = 0; i < PLAYERS; i++)
	{
		char cbuf[64];
		// �÷��̾� ü��
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

		LHPutAttrCharExLine(162, i * 20 + 5, "�����������", color);

		// �÷��̾� ��ź ����
		sprintf_s(cbuf, 64, "BOMB LEVEL     : %d", player[i].bomb_level);
		LHPutAttrCharEx(163, i * 20 + 8, cbuf, black, white);
		memset(cbuf, 0, 64);
		for (int j = 0; j < player[i].bomb_level; j++)
		{
			strcat_s(cbuf, 64, "��");
		}
		LHPutAttrCharEx(162, i * 20 + 10, cbuf, black, red);

		// �÷��̾� �ִ� ��ź ����
		sprintf_s(cbuf, 64, "BOMB MAX COUNT : %d", player[i].bomb_max_count);
		LHPutAttrCharEx(163, i * 20 + 13, cbuf, black, white);
		memset(cbuf, 0, 64);
		for (int j = 0; j < player[i].bomb_max_count; j++)
		{
			strcat_s(cbuf, 64, "��");
		}
		LHPutAttrCharEx(162, i * 20 + 15, cbuf, black, brown);
	}
}

/// <summary>
/// ĳ���Ͱ� �̵� �������� Ȯ���ϴ� �Լ�
/// </summary>
bool Check_Move(int x, int y)
{
	// �ʿ��� ����ִ��� Ȯ��
	if (tempmap[y][x] != '0')
	{
		return false;
	}

	// ĳ���Ͱ� �ִ��� Ȯ��
	for (int i = 0; i < PLAYERS; i++)
	{
		if (player[i].x == x && player[i].y == y)
		{
			return false;
		}
	}

	return true;
}