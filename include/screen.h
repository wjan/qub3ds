#ifndef SCREEN_H
#define SCREEN_H

#include <drawable.h>

#define STATE_GAME 1
#define STATE_MENU 0
#define STATE_PAUSE 2

class Screen
{
	public:
		Screen();
		void draw();
		int state;
		~Screen();

};

#endif
