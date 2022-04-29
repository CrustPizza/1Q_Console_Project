#ifndef RENDER_H
#define RENDER_H
#include "render.h"
#endif


#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "controller.h"
#endif

#ifndef KEY_STATE_H
#define KEY_STATE_H
enum Key_state
{
	UP,
	DOWN,
	STAY
};
#endif

void MaplistDraw_sample(const int x, const int y, const int select);
void infoDraw_sample(const int x, const int y);
void MenuDraw_sample(const int x, const int y, const int select);
void titleDraw_sample();