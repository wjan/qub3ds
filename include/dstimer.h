#ifndef DSTIMER_H
#define DSTIMER_H

#include <dstimerhandler.h>

class DSTimer
{
	public:
		DSTimer(int _initTicks, DSTimerHandler *handler, char *name);
		~DSTimer();
		int initTicks;
		int ticksLeft;
		char *name;
		DSTimerHandler *handler;

};

#endif
