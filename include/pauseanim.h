#ifndef PAUSEANIM_H
#define PAUSEANIM_H

#include <game.h>
#include <animation.h>

#include <parameters.h>

class PauseAnim : public Animation
{
	public:
		PauseAnim(bool pause, Game *game);
		~PauseAnim();
		bool isBlocking();
		void performEndAction();
		void draw ( Drawer *drawer );
	private:
		bool pause;
		Game *game;

};

#endif
