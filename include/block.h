#ifndef BLOCK_H
#define BLOCK_H

#include <gamedata.h>

#define BLOCK_UP 0
#define BLOCK_RIGHT 1
#define BLOCK_LEFT 2
#define BLOCK_DOWN 3
#define BLOCK_TOP 4
#define BLOCK_STUCK 5

class Block
{
	public:
		Block();
		void spawn ( int x, int y, int type, bool isLeft );
		~Block();

		int type;
		int position;
		int posX;
		int posY;
		int posZ;
		bool active;
};

#endif
