#ifndef PATTERNANIM_H
#define PATTERNANIM_H

#include <animation.h>
#include <drawer.h>
#include <board.h>

class PatternElement
{
	public:
		int x, y, size, delay;
};

class PatternAnim : public Animation
{
	public:
		PatternAnim ( int ticks );
		~PatternAnim();
		void addPatternElement ( int x, int y, int size );
		bool isBlocking();
		void draw ( Drawer *drawer );
	private:
		int maxDelay;
		int initialTicks;
		PatternElement *elements[BOARD_WIDTH * BOARD_HEIGHT];
		int elementsNo;
};

#endif
