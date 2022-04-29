#include "stdafx.h"
#include "bomb.h"
#include <list>

#define TIMER 2000
#define MAX_BOMB_COUNT 5
#define MAX_HEALTH 100
#define PLUS_HEALTH 10
#define BOMB_MAX_LEVEL 7

using std::list;

struct BombQueue
{
	struct Bomb BombArr[16];
	int size;
	int front;
	int rear;
};

struct BCOORD
{
	int x;
	int y;
};

struct BCOORD BombCheck[16];

struct BombQueue bombQ;

typedef struct bomb_effect
{
	int x;
	int y;
	int color;
	ULONGLONG timer;
};

list<bomb_effect> effect_list;

int IsEmptyQ() {
	if (bombQ.front == bombQ.rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//처음 rec = 1, rec == power 이면 리턴  
void left_recursive(int rec, int power)
{
	struct Bomb bomb = bombQ.BombArr[bombQ.front];
	char object = tempmap[bomb.y][bomb.x - rec];

	if (power == 0)
	{
		return;
	}

	if (rec > power)
	{
		return;
	}


	if (object == '1')
	{
		return;
	}

	if (bomb.player == 0)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x - rec && player[0].y == bomb.y)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x - rec && player[1].y == bomb.y)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'B': //blue
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y][bomb.x - rec] = '0';
			if (player[0].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[0].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[0].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'C':
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y][bomb.x - rec] = '0';
			if (player[0].bomb_level < BOMB_MAX_LEVEL)
			{
				player[0].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'D':
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y][bomb.x - rec] = '0';
			if (player[0].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[0].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'W': //yellow
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			tempmap[bomb.y][bomb.x - rec] = '0';
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'X': //yellow
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			tempmap[bomb.y][bomb.x - rec] = '0';
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Y': //yellow
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			tempmap[bomb.y][bomb.x - rec] = '0';
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Z': //yellow
		{
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			tempmap[bomb.y][bomb.x - rec] = '0';
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (bomb.player == 1)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x - rec && player[0].y == bomb.y)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x - rec && player[1].y == bomb.y)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'B':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'C':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'D':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'W':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'X':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			if (player[1].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[1].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[1].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Y':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_level < BOMB_MAX_LEVEL)
			{
				player[1].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Z':
		{
			tempmap[bomb.y][bomb.x - rec] = '0';
			effect_list.push_back({ (bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x - rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[1].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}

	return left_recursive(++rec, power);
}

void right_recursive(int rec, int power)
{
	struct Bomb bomb = bombQ.BombArr[bombQ.front];
	char object = tempmap[bomb.y][bomb.x + rec];

	if (power == 0)
	{
		return;
	}

	if (rec > power)
	{
		return;
	}


	if (object == '1')
	{
		return;
	}

	if (bomb.player == 0)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x + rec && player[0].y == bomb.y)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x + rec && player[1].y == bomb.y)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'B': //blue
		{
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y][bomb.x + rec] = '0';
			if (player[0].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[0].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[0].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'C':
		{
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y][bomb.x + rec] = '0';
			if (player[0].bomb_level < BOMB_MAX_LEVEL)
			{
				player[0].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'D':
		{
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y][bomb.x + rec] = '0';
			if (player[0].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[0].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'W': //yellow
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'X': //yellow
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Y': //yellow
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Z': //yellow
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (bomb.player == 1)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x + rec && player[0].y == bomb.y)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x + rec && player[1].y == bomb.y)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'B':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'C':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'D':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'W':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'X':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			if (player[1].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[1].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[1].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Y':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_level < BOMB_MAX_LEVEL)
			{
				player[1].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Z':
		{
			tempmap[bomb.y][bomb.x + rec] = '0';
			effect_list.push_back({ (bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel((bomb.x + rec) * X_Multiplier, bomb.y * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[1].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}

	return right_recursive(++rec, power);
}

void up_recursive(int rec, int power)
{
	struct Bomb bomb = bombQ.BombArr[bombQ.front];
	char object = tempmap[bomb.y - rec][bomb.x];

	if (power == 0)
	{
		return;
	}
	if (rec > power)
	{
		return;
	}
	if (object == '1')
	{
		return;
	}

	if (bomb.player == 0)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x && player[0].y == bomb.y - rec)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x && player[1].y == bomb.y - rec)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'B': //blue
		{
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y - rec][bomb.x] = '0';
			if (player[0].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[0].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[0].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'C':
		{
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y - rec][bomb.x] = '0';
			if (player[0].bomb_level < BOMB_MAX_LEVEL)
			{
				player[0].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'D':
		{
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y - rec][bomb.x] = '0';
			if (player[0].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[0].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'W': //yellow
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'X': //yellow
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Y': //yellow
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Z': //yellow
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (bomb.player == 1)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x && player[0].y == bomb.y - rec)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x && player[1].y == bomb.y - rec)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'B':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'C':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'D':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'W':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'X':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			if (player[1].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[1].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[1].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Y':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_level < BOMB_MAX_LEVEL)
			{
				player[1].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Z':
		{
			tempmap[bomb.y - rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y - rec) * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[1].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return up_recursive(++rec, power);
}

void down_recursive(int rec, int power)
{
	struct Bomb bomb = bombQ.BombArr[bombQ.front];
	char object = tempmap[bomb.y + rec][bomb.x];

	if (power == 0)
	{
		return;
	}

	if (rec > power)
	{
		return;
	}

	if (object == '1')
	{
		return;
	}

	if (bomb.player == 0)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x && player[0].y == bomb.y + rec)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x && player[1].y == bomb.y + rec)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'B': //blue
		{
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y + rec][bomb.x] = '0';
			if (player[0].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[0].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[0].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'C':
		{
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y + rec][bomb.x] = '0';
			if (player[0].bomb_level < BOMB_MAX_LEVEL)
			{
				player[0].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'D':
		{
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			tempmap[bomb.y + rec][bomb.x] = '0';
			if (player[0].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[0].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'W': //yellow
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'X': //yellow
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Y': //yellow
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		case 'Z': //yellow
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, cyan, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, cyan);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	else if (bomb.player == 1)
	{
		switch (object)
		{
		case '0':
		{
			if (player[0].x == bomb.x && player[0].y == bomb.y + rec)
			{
				player[0].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			else if (player[1].x == bomb.x && player[1].y == bomb.y + rec)
			{
				player[1].health -= bomb.power * 5;
				PlayEFFECT(HIT);
			}
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case '4':
		{
			return;
		}
		case 'A':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'B':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'C':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'D':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'W':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			break;
		}
		case 'X':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			if (player[1].health + PLUS_HEALTH > MAX_HEALTH)
			{
				player[1].health = 100;
				PlayEFFECT(LEVEL_UP);
			}
			else
			{
				player[1].health += PLUS_HEALTH;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Y':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_level < BOMB_MAX_LEVEL)
			{
				player[1].bomb_level += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		case 'Z':
		{
			tempmap[bomb.y + rec][bomb.x] = '0';
			effect_list.push_back({ bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, yellow, GetTickCount64() });
			//Draw_nPixel(bomb.x * X_Multiplier, (bomb.y + rec) * Y_Multiplier, BOMB, black, yellow);
			if (player[1].bomb_max_count < MAX_BOMB_COUNT)
			{
				player[1].bomb_max_count += 1;
				PlayEFFECT(LEVEL_UP);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return down_recursive(++rec, power);
}

void Bomb_Init()
{
	bombQ.size = 16;
	bombQ.front = 0;
	bombQ.rear = 0;
	effect_list.clear();
}

void create_bomb(Character* chr) //플레이어가 폭탄을 설치할때 호출
{
	for (int i = bombQ.front; i != bombQ.rear; i = (i + 1) % bombQ.size)
	{
		if (bombQ.BombArr[i].x == chr->x && bombQ.BombArr[i].y == chr->y)
		{
			return;
		}
	}

	if (chr->bomb_count < chr->bomb_max_count)
	{
		PlayEFFECT(BOMB_CREATE);

		tempmap[chr->y][chr->x] = '2' + chr->number;
		bombQ.BombArr[bombQ.rear].start_time = (long int)(GetTickCount64());
		bombQ.BombArr[bombQ.rear].power = chr->bomb_level;
		bombQ.BombArr[bombQ.rear].x = chr->x;
		bombQ.BombArr[bombQ.rear].y = chr->y;
		bombQ.BombArr[bombQ.rear].player = chr->number;
		bombQ.rear = (bombQ.rear + 1) % bombQ.size;
		chr->bomb_count++;
	}
}

void explode_bomb() //폭탄이 터질때 호출
{
	struct Bomb bomb = bombQ.BombArr[bombQ.front];
	tempmap[bomb.y][bomb.x] = '0';
	player[bomb.player].bomb_count--;
	int power = bomb.power;
	// '0' or '1'

	if (player[0].x == bomb.x && player[0].y == bomb.y)
	{
		player[0].health -= bomb.power * 5;
		PlayEFFECT(HIT);
	}
	else if (player[1].x == bomb.x && player[1].y == bomb.y)
	{
		player[1].health -= bomb.power * 5;
		PlayEFFECT(HIT);
	}

	effect_list.push_back({ bomb.x * X_Multiplier, bomb.y * Y_Multiplier, bomb.player ? yellow : cyan, GetTickCount64() });

	PlayEFFECT(BOMB_EXPLODE);
	
	left_recursive(1, power);
	right_recursive(1, power);
	up_recursive(1, power);
	down_recursive(1, power);
}

void Bomb_Update()
{
	if (!IsEmptyQ())
	{
		for (int i = 0; i < 16; i++)
		{
			if (tempmap[bombQ.BombArr[i].y][bombQ.BombArr[i].x] == '2')
			{
				Draw_nPixel(bombQ.BombArr[i].x * X_Multiplier, bombQ.BombArr[i].y * Y_Multiplier, BOMB, black, cyan);
			}
			else if (tempmap[bombQ.BombArr[i].y][bombQ.BombArr[i].x] == '3')
			{
				Draw_nPixel(bombQ.BombArr[i].x * X_Multiplier, bombQ.BombArr[i].y * Y_Multiplier, BOMB, black, yellow);
			}
		}

		if ((long int)GetTickCount64() - bombQ.BombArr[bombQ.front].start_time > TIMER)
		{
			explode_bomb();
			bombQ.front = (bombQ.front + 1) % bombQ.size;
		}
	}

	for (list<bomb_effect>::iterator iter = effect_list.begin(); iter != effect_list.end(); )
	{
		if (GetTickCount64() - iter->timer < 200)
		{
			Draw_nPixel(iter->x, iter->y, BOMB, black, iter->color);
			iter++;
		}
		else
		{
			iter = effect_list.erase(iter);
		}
	}
}