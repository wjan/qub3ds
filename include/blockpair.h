#ifndef BLOCKPAIR_H
#define BLOCKPAIR_H

#include <block.h>
#include <board.h>
#include <gamedata.h>
#include <animationmanager.h>

#include <parameters.h>

class BlockPair
{
	public:
		BlockPair(GameData gd);
		GameData gameData;
		void spawn(int x, int y, int type1, int type2);
		bool move(int dir, Board *board);
		bool isSettled();
		void draw(Drawer *drawer);
		~BlockPair();
		Block bl1;
		Block bl2;
		AnimationManager *anims;
	private:
		bool moveSingle(int dir, Board *board);
		void bottomCollision(Board *board);
		bool isBlockUseless(Block *bl1, Block *bl2, Board *board);
		float getLighterColor(float color);
};

#endif
