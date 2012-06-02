#include <dstimer.h>

DSTimer::DSTimer ( int _initTicks, DSTimerHandler *_handler, char *_name )
{
	initTicks = _initTicks;
	ticksLeft = _initTicks;
	name = _name;
	handler = _handler;
}


DSTimer::~DSTimer()
{
}
