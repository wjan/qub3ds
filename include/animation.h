#ifndef ANIMATION_H
#define ANIMATION_H

#include <drawable.h>

class Animation : public Drawable
{
	public:
		int ticksLeft;
		void tick();
		bool anyTicksLeft();
		virtual bool isBlocking() {};
		virtual void performEndAction() {};
};

#endif
