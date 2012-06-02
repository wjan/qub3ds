#ifndef GAME_H
#define GAME_H

#include <gamedata.h>
#include <board.h>
#include <level.h>
#include <blockpair.h>
#include <dstimersmanager.h>
#include <dstimerhandler.h>
#include <brickdisanim.h>
#include <animationmanager.h>
#include <stylusgesture.h>

#include <parameters.h>

class Game : DSTimerHandler
{
	public:
		Game();
		~Game();
		void tick();
		void reset();
		void start();
		void notify(char *name);
		void fallPhase();
		void animPhase();
		bool isOver();
		void turnPaused();
		int getScore();
		int highscore;
	private:
		Drawer drawer;
		Level level;
		GameData gameData;
		Board *board;
		BlockPair *blocks;
		DSTimersManager timers;
		void moveBlock(int dir);
		void blockFalling();
		void handleInput();
		void unblockInput();
		int inputBlocked;
		AnimationManager anims;
		AnimationManager miscAnims;
		bool fallCheck;
		bool paused;
		bool over;
		bool end;
		StylusGesture gesture;
};

#endif
