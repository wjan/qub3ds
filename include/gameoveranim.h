#ifndef GAMEOVERANIM_H
#define GAMEOVERANIM_H

#include <animation.h>

#include <parameters.h>

class GameOverAnim : public Animation
{
	public:
		GameOverAnim();
		~GameOverAnim();
		bool isBlocking();
		void draw ( Drawer *drawer );

};

#endif