#include <block.h>

Block::Block()
{
}

void Block::spawn(int x, int y, int _type, bool isLeft) {

	posX = x;
	posY = y;
	posZ = 0;
	type = _type;
	active = true;
	if (isLeft) {
		position = BLOCK_LEFT;
	} else {
		position = BLOCK_RIGHT;
	}
}

Block::~Block()
{
}
