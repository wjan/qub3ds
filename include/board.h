#ifndef BOARD_H
#define BOARD_H

#define TYPE_SQUARE 1
#define TYPE_ELSE 2
#define BLOCK_NONE -1

#include <settledblock.h>
#include <drawable.h>
#include <nds.h>
#include <gamedata.h>
#include <level.h>
#include <animationmanager.h>

#include <parameters.h>

class Board : Drawable
{
	public:
		Board ( GameData gd );
		~Board();
		GameData gameData;
		Level *level;
		void reset();
		bool isCollidable ( int x, int y );
		bool isBottom ( int y );
		void brickSettled ( int x, int y, int z, int type );
		SettledBlock blocks[BOARD_WIDTH][BOARD_HEIGHT];
		void draw ( Drawer *drawer );
		bool findPatterns ( AnimationManager *anims );
		void letBricksFall ( AnimationManager *anims );
		int animBeat;
		bool isGameOver();
		bool isOutside ( int y );
		bool fallingSequenceEnded;
	private:
		int p1[BOARD_WIDTH][BOARD_HEIGHT];
		int p2[BOARD_WIDTH][BOARD_HEIGHT];
		int p3[BOARD_WIDTH][BOARD_HEIGHT];
		void makeAStep ( int x, int y, int walkNo, int searchType, int tempBoard[BOARD_WIDTH][BOARD_HEIGHT], int counts[BOARD_WIDTH * BOARD_HEIGHT], int types[BOARD_WIDTH * BOARD_HEIGHT] );
		void makeAStepForSquare ( int x, int y, int walkNo, int searchType, int tempBoard[BOARD_WIDTH][BOARD_HEIGHT], int counts[BOARD_WIDTH * BOARD_HEIGHT], int types[BOARD_WIDTH * BOARD_HEIGHT] );
};

#endif
