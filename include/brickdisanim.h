#ifndef BRICKDISANIM_H
#define BRICKDISANIM_H

#include <animation.h>
#include <drawer.h>

class BrickDisAnim : public Animation
{
	public:
		BrickDisAnim(int x, int y, int z, int ticks);
		~BrickDisAnim();
		bool isBlocking();
		void draw(Drawer *drawer);
	private:
		int x, y, z, initialTicks;

};

#endif
