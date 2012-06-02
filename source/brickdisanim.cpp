#include <brickdisanim.h>


BrickDisAnim::BrickDisAnim(int _x, int _y, int _z, int ticks)
{
	x = _x;
	y = _y;
	z = _z;
	ticksLeft = ticks;
	initialTicks = ticks;
}

bool BrickDisAnim::isBlocking()
{
	return false;
}

void BrickDisAnim::draw ( Drawer *drawer )
{
	float fraction = (float)ticksLeft / initialTicks;
	drawer->drawBrick(x, y, z, 1, 1, fraction,  fraction, fraction);
}

BrickDisAnim::~BrickDisAnim()
{
}
