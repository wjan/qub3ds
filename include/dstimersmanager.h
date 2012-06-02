#ifndef DSTIMERS_H
#define DSTIMERS_H

#include <dstimer.h>
#include <dstimerhandler.h>
#include <stdlib.h>

#define TIMERS_SLOTS 20

class DSTimersManager
{
	public:
		DSTimersManager();
		~DSTimersManager();
		void addTimer ( int ticks, DSTimerHandler *handler, char *name );
		void stopTimer( char *name );
		void tick();
		void reset();
		int used;
		int slots;
		DSTimer *timers[TIMERS_SLOTS];
		void removeTimer ( char *name );

	private:
		void removeTimer ( int timerId );

};

#endif
