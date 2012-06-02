#include <pauseanim.h>
#include <stdio.h>

PauseAnim::PauseAnim(bool _pause, Game *g)
{
	ticksLeft = PAUSE_ANIM_TIME;
	pause = _pause;
	game = g;
	
	if (!pause) {
		// set paused at the beginning of animation
		game->turnPaused();
	}
}

void PauseAnim::draw ( Drawer *drawer )
{
	if (!pause) {
		drawer->z -= 5;
	} else {
		drawer->z += 5;
	}
}

bool PauseAnim::isBlocking()
{
	return true;
}

void PauseAnim::performEndAction() {
	if (pause) {
		// unpause at the end of animation
		game->turnPaused();
	}
}

PauseAnim::~PauseAnim()
{
}
