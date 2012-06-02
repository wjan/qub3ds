#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <animation.h>
#include <drawer.h>

#define ANIM_MAX 20

class AnimationManager
{
	public:
		AnimationManager();
		~AnimationManager();
		Animation *animations[ANIM_MAX];
		void addAnimation(Animation *anim);
		void draw(Drawer *drawer);
		void tick();
		bool isBlocking();
	private:
		int used;
};

#endif
