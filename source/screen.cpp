//TODO: not used right now
#include <screen.h>
#include <stdio.h>

Screen::Screen()
{
}

void Screen::draw() {
	if (state == STATE_MENU) {
		printf("\x1b[1;1HPress START to begin\n");
	}
}

Screen::~Screen()
{
}
