#include "stdafx.h"
#include "map.h"

char tempmap[map_h][map_w];

char map1[map_h][map_w] = {};

void MapCreate()
{
	for (int h = 1; h < map_h - 1; h++)
	{
		for (int w = 1; w < map_w - 1; w++)
		{
			// 블럭 생성 40%
			int rd = rand() % OBS_FREQUENCY;

			if (rd < 2)
			{
				// 0 ~ 13
				int random = rand() % 13;

				// A, B, C, D 파랑 블럭
				if (random < 4)
				{
					// random 0 ~ 3
					tempmap[h][w] = 'A' + random;
				}
				// W, X, Y, Z 노랑 블럭
				else if (random < 8)
				{
					// random 4 ~ 7
					tempmap[h][w] = 'Z' + 4 - random;
				}
				// '4'는 회색 블럭
				else
				{
					tempmap[h][w] = '4';
				}
			}
		}
	}

	// 플레이어들을 위한 시작 여유 공간
	for (int y = 1; y < 3; y++)
	{
		for (int x = 1; x < 3; x++)
		{
			tempmap[y][x] = '0';
			tempmap[map_h - 1 - y][map_w - 1 - x] = '0';
		}
	}
}

void map_init()
{
	// 기본 테두리 벽을 위한 초기화
	for (int y = 0; y < map_h; y++)
	{
		for (int x = 0; x < map_w; x++)
		{
			if (y == 0 || x == 0 || y == map_h - 1 || x == map_w - 1)
			{
				map1[y][x] = '1';
			}
			else
			{
				map1[y][x] = '0';
			}
		}
	}

	memcpy(tempmap, map1, sizeof(map1));
	MapCreate();
}