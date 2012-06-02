#include <gameoveranim.h>

GameOverAnim::GameOverAnim()
{
	ticksLeft = GAMEOVER_TIME;
}

void GameOverAnim::draw ( Drawer *drawer )
{
	drawer->z -= 0.4;
	drawer->y -= 0.2;
}

bool GameOverAnim::isBlocking()
{
	return true;
}

GameOverAnim::~GameOverAnim()
{
}
