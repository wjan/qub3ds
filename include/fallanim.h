#ifndef FALLANIM_H
#define FALLANIM_H

#include <animation.h>
#include <drawer.h>
#include <board.h>
#include <gamedata.h>

#include <parameters.h>

class FallAnim : public Animation
{
	public:
		FallAnim ( int ticks, GameData gd );
		~FallAnim();
		bool isBlocking();
		void draw ( Drawer *drawer );
		void addFallElement ( int x, int y, int type );
	private:
		int fallElements[BOARD_WIDTH][BOARD_HEIGHT];
		int initialTicks;
		GameData gameData;
};

#endif
