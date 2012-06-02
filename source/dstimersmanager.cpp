#include <dstimersmanager.h>
#include <string.h>
#include <stdio.h>

DSTimersManager::DSTimersManager()
{
	//TODO: parametrized in runtime?
	slots = TIMERS_SLOTS;
	used = 0;
}

void DSTimersManager::removeTimer( char *name ) {
	for (int i = 0; i < slots; i++) {
		if (strcmp(timers[i]->name, name) == 0) {
			removeTimer (i);
			break;
		}
	}
}

void DSTimersManager::addTimer ( int ticks, DSTimerHandler *handler, char *name )
{
	if ( used > slots )
	{
		//TODO: need to allocate another slot!
	}
	else
	{
		DSTimer *timer = new DSTimer ( ticks, handler, name );;
		timers[used] = timer;
		used++;
	}
}

void DSTimersManager::stopTimer(char *name) {
	for (int i = 0; i < used; i++) {
		if (strcmp(timers[i]->name, name) == 0) {
			removeTimer(i);
		}
	}
}

void DSTimersManager::removeTimer ( int timerId )
{
	delete timers[timerId];
	if ( timerId + 1 != used )
	{
		timers[timerId] = timers[used - 1];
		timers[used - 1] = NULL;
	}
	else
	{
		//no need to swap with dead timer
	}
	used--;
}

void DSTimersManager::tick()
{
	for ( int i = 0; i < used; i++ )
	{
		timers[i]->ticksLeft--;
		if ( timers[i]->ticksLeft == 0 )
		{

			if ( timers[i]->handler != NULL )
			{
				timers[i]->handler->notify(timers[i]->name);
			}
			else
			{
				//TODO: error cause function pointer is null
			}
			removeTimer ( i );
		}
	}
}

void DSTimersManager::reset() {
	for (int i = 0; i < used; i++) {
		removeTimer(i);
	}
}

DSTimersManager::~DSTimersManager()
{
}
