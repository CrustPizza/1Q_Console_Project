#include "stdafx.h"
#include "render.h"

// 스크린 버퍼 핸들
static HANDLE	screen_buffer[2];

// 현재 스크린 인덱스
static int		index;

// 창 크기
const int		cols = 185;
const int		lines = 40;

/// <summary>
/// Buffer screen 초기화
/// </summary>
void screen_init()
{
	// Util에 있던 내용 여기로 이동
	char buffer[128];
	sprintf_s(buffer, 128, "mode con cols=%d lines=%d | title 게임      ", cols, lines);
	system(buffer); //cols:가로, lines: 세로, title: 창 제목

	// 커서 크기, 투명 설정
	CONSOLE_CURSOR_INFO cursor_info;
	cursor_info.bVisible = 0;
	cursor_info.dwSize = 1;

	index = 0;

	// 스크린 버퍼 초기화
	for (int i = 0; i < 2; i++)
	{
		screen_buffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(screen_buffer[i], &cursor_info);
	}
}

/// <summary>
/// Buffer screen 해제
/// </summary>
void screen_release()
{
	for (int i = 0; i < 2; i++)
	{
		CloseHandle(screen_buffer[i]);
	}
}

/// <summary>
/// Screen 초기화
/// </summary>
void screen_clear()
{
	DWORD dw;
	COORD xy = { 0,0 };

	// 버퍼 크기만큼 지워준다.
	FillConsoleOutputAttribute(screen_buffer[index], 7, cols * lines, xy, &dw);
	FillConsoleOutputCharacter(screen_buffer[index], ' ', cols * lines, xy, &dw);
}

/// <summary>
/// 좌표에 문자열 출력 ( 컬러속성설정 )
/// </summary>
void LHPutAttrCharEx(const int x, const int y, const char* pStr, const WORD bg, const WORD fg)
{
	const COORD xy = { x, y };
	const int   nStrlen = strlen(pStr);
	DWORD		dw;
	WORD		temp[256];

	// 연산을 줄이기 위한 컬러 배열
	for (int i = 0; i < nStrlen; i++)
	{
		temp[i] = bg * 16 + fg;
	}

	// 컬러 입히기
	WriteConsoleOutputAttribute(screen_buffer[index], temp, nStrlen, xy, &dw);
	// 문자 출력
	WriteConsoleOutputCharacterA(screen_buffer[index], pStr, nStrlen, xy, &dw);
}

// 연산 문제 때문에 한줄을 출력하는 함수를 따로 만들었다.
void LHPutAttrCharExLine(const int x, const int y, const char* pStr, WORD* color)
{
	const COORD xy = { x, y };
	const int   nStrlen = strlen(pStr);
	DWORD		dw;

	WriteConsoleOutputAttribute(screen_buffer[index], color, nStrlen, xy, &dw);
	WriteConsoleOutputCharacterA(screen_buffer[index], pStr, nStrlen, xy, &dw);
}

/// <summary>
/// Buffer screen 교체
/// </summary>
void screen_flipping()
{
	// front와 back 교체
	SetConsoleActiveScreenBuffer(screen_buffer[index]);
	index = !index;
}

/// <summary>
/// Text buffer에 저장된 내용을 Back buffer에 그려넣고 Front와 교체
/// </summary>
void render()
{
	screen_flipping();
	screen_clear();
}