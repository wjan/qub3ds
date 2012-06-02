#include <animation.h>

void Animation::tick()
{
	ticksLeft--;
}

bool Animation::anyTicksLeft()
{
	return ticksLeft > 0;
}
