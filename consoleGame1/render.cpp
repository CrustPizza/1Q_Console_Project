#include "stdafx.h"
#include "render.h"

// ��ũ�� ���� �ڵ�
static HANDLE	screen_buffer[2];

// ���� ��ũ�� �ε���
static int		index;

// â ũ��
const int		cols = 185;
const int		lines = 40;

/// <summary>
/// Buffer screen �ʱ�ȭ
/// </summary>
void screen_init()
{
	// Util�� �ִ� ���� ����� �̵�
	char buffer[128];
	sprintf_s(buffer, 128, "mode con cols=%d lines=%d | title ����      ", cols, lines);
	system(buffer); //cols:����, lines: ����, title: â ����

	// Ŀ�� ũ��, ���� ����
	CONSOLE_CURSOR_INFO cursor_info;
	cursor_info.bVisible = 0;
	cursor_info.dwSize = 1;

	index = 0;

	// ��ũ�� ���� �ʱ�ȭ
	for (int i = 0; i < 2; i++)
	{
		screen_buffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(screen_buffer[i], &cursor_info);
	}
}

/// <summary>
/// Buffer screen ����
/// </summary>
void screen_release()
{
	for (int i = 0; i < 2; i++)
	{
		CloseHandle(screen_buffer[i]);
	}
}

/// <summary>
/// Screen �ʱ�ȭ
/// </summary>
void screen_clear()
{
	DWORD dw;
	COORD xy = { 0,0 };

	// ���� ũ�⸸ŭ �����ش�.
	FillConsoleOutputAttribute(screen_buffer[index], 7, cols * lines, xy, &dw);
	FillConsoleOutputCharacter(screen_buffer[index], ' ', cols * lines, xy, &dw);
}

/// <summary>
/// ��ǥ�� ���ڿ� ��� ( �÷��Ӽ����� )
/// </summary>
void LHPutAttrCharEx(const int x, const int y, const char* pStr, const WORD bg, const WORD fg)
{
	const COORD xy = { x, y };
	const int   nStrlen = strlen(pStr);
	DWORD		dw;
	WORD		temp[256];

	// ������ ���̱� ���� �÷� �迭
	for (int i = 0; i < nStrlen; i++)
	{
		temp[i] = bg * 16 + fg;
	}

	// �÷� ������
	WriteConsoleOutputAttribute(screen_buffer[index], temp, nStrlen, xy, &dw);
	// ���� ���
	WriteConsoleOutputCharacterA(screen_buffer[index], pStr, nStrlen, xy, &dw);
}

// ���� ���� ������ ������ ����ϴ� �Լ��� ���� �������.
void LHPutAttrCharExLine(const int x, const int y, const char* pStr, WORD* color)
{
	const COORD xy = { x, y };
	const int   nStrlen = strlen(pStr);
	DWORD		dw;

	WriteConsoleOutputAttribute(screen_buffer[index], color, nStrlen, xy, &dw);
	WriteConsoleOutputCharacterA(screen_buffer[index], pStr, nStrlen, xy, &dw);
}

/// <summary>
/// Buffer screen ��ü
/// </summary>
void screen_flipping()
{
	// front�� back ��ü
	SetConsoleActiveScreenBuffer(screen_buffer[index]);
	index = !index;
}

/// <summary>
/// Text buffer�� ����� ������ Back buffer�� �׷��ְ� Front�� ��ü
/// </summary>
void render()
{
	screen_flipping();
	screen_clear();
}