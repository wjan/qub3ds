#include <game.h>
#include <drawer.h>
#include <brickdisanim.h>
#include <level.h>
#include <nds.h>
#include <string.h>
#include <stdio.h>
#include <pauseanim.h>
#include <gameoveranim.h>
#include <soundmanager.h>
#include <debug.h>

#define TIMER_FALLING "blockFalling"
#define TIMER_UNBLOCK_INPUT "unblockInput"

Game::Game()
{
	blocks = new BlockPair ( gameData );
	blocks->anims = &anims;
	board = new Board ( gameData );
	board->level = &level;
	powerON ( POWER_ALL );
	videoSetMode ( MODE_0_3D | DISPLAY_BG1_ACTIVE );
	vramSetBankC ( VRAM_C_MAIN_BG_0x06000000 );

	BG_PALETTE[255] = 0xFFFF;
	BG1_CR = BG_MAP_BASE ( 31 ) | BG_PRIORITY ( 0 ); // use bg 1 for text, set to highest priority
	BG0_CR = BG_PRIORITY ( 1 ); //set bg 0 (3d background) to be a lower priority than bg 1

	consoleInitDefault ( ( u16* ) SCREEN_BASE_BLOCK ( 31 ), ( u16* ) CHAR_BASE_BLOCK ( 0 ), 16 );

	irqInit();
	irqSet ( IRQ_VBLANK, 0 );
	glInit();
	glEnable ( GL_ANTIALIAS );
	glClearColor ( 0,0,0,31 ); // BG must be opaque for AA to work
	glClearPolyID ( 63 ); // BG must have a unique polygon ID for AA to work
	glClearDepth ( 0x7FFF );
	glViewport ( 0,0,255,191 );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective ( 70, 256.0 / 192.0, 0.1, 100 );
	glPolyFmt ( POLY_ALPHA ( 31 ) | POLY_CULL_NONE );
	glMatrixMode ( GL_MODELVIEW );

}

void Game::tick()
{

	if ( !end && !paused && !anims.isBlocking() )
	{
		static bool firstPattern = true;
		if ( fallCheck )
		{
			board->letBricksFall ( &anims );
			if ( board->fallingSequenceEnded )
			{
				fallCheck = false;
				firstPattern = false;
			}
		}
		if ( !blocks->isSettled() )
		{
			firstPattern = true;
		}
		if ( board->fallingSequenceEnded && blocks->isSettled() )
		{
			if ( board->findPatterns ( &anims ) )
			{
				SoundManager::getInstance().patternCreated ( firstPattern );
				fallCheck = true;
			}
			else
			{
				blocks->spawn ( GAME_SPAWN_X, GAME_SPAWN_Y, level.bl1type, level.bl2type );
				if ( !end && board->isGameOver() )
				{
					SoundManager::getInstance().gameover();
					end = true;
					GameOverAnim *goanim = new GameOverAnim();
					miscAnims.addAnimation ( goanim );
					consoleClear();
				}
				level.notifyFallingSequenceEnded();
				level.drawNextBlocks();
			}
		}

		timers.tick();

	}

	if ( !miscAnims.isBlocking() )
	{
		handleInput();
	}

	if ( over )
	{
		// input handling could bring you game exit
		return;
	}

	miscAnims.tick();

	if ( !paused )
	{
		anims.tick();
	}

	BrickColor *b1n = gameData.getBrickColor ( level.bl1type );
	BrickColor *b2n = gameData.getBrickColor ( level.bl2type );
	drawer.draw();

	board->draw ( &drawer );
	blocks->draw ( &drawer );
	anims.draw ( &drawer );
	miscAnims.draw ( &drawer );

	if ( !paused && !end )
	{
		drawer.drawBrick ( GAME_NEXT_BRICK_POSITION_X, GAME_NEXT_BRICK_POSITION_Y, GAME_NEXT_BRICK_POSITION_Z, 1, 1, b1n->r, b1n->g, b1n->b );
		drawer.drawBrick ( GAME_NEXT_BRICK_POSITION_X + 1, GAME_NEXT_BRICK_POSITION_Y, GAME_NEXT_BRICK_POSITION_Z, GAME_NEXT_BRICK_POSITION_SIZE, 1, b2n->r, b2n->g, b2n->b );
		printf ( "\x1b[1;1HNext\n" );
		printf ( "\x1b[1;26HScore\n" );
		printf ( "\x1b[3;26H%d\n", level.score );
		printf ( "\x1b[5;26HLevel\n" );
		printf ( "\x1b[7;26H%d\n", level.level );
		printf ( "\x1b[7;1HRecord\n" );
		printf ( "\x1b[9;1H%d\n", highscore );
		printf ( "\x1b[10;26HMusic\n" );
		if ( SoundManager::getInstance().isMusicHearable() )
		{
			printf ( "\x1b[12;26H%s\n", "On " );
		}
		else
		{
			printf ( "\x1b[12;26H%s\n", "Off" );
		}
	}
	else if ( paused )
	{
		printf ( "\x1b[3;10HGame paused\n" );
		printf ( "\x1b[5;4HPress START to unpause\n" );
		printf ( "\x1b[7;5HPress SELECT to quit\n" );
	}
	else if ( end )
	{
		printf ( "\x1b[3;10HGame over!\n" );
		printf ( "\x1b[5;7HYour score is %d\n", level.score );
		printf ( "\x1b[7;4HPress START to continue\n" );
	}

	glFlush ( 0 );
	swiWaitForVBlank();
}

void Game::reset()
{
	consoleClear();
	board->reset();
	level.reset();
	timers.reset();
	drawer.reset();
	blocks->spawn ( GAME_SPAWN_X, GAME_SPAWN_Y, level.bl1type, level.bl2type );
	level.drawNextBlocks();
	fallCheck = false;
	paused = false;
	over = false;
	end = false;
	Debug::getInstance().configureDrawer ( &drawer );
}

void Game::start()
{
	timers.addTimer ( GAME_SPEED, this, TIMER_FALLING );
}

void Game::moveBlock ( int dir )
{
	if ( blocks->move ( dir, board ) )
	{
		SoundManager::getInstance().blockMooved();
	}
}

void Game::unblockInput()
{
	inputBlocked = 0;
}

void Game::handleInput()
{
	scanKeys();
	PA_CheckLid();

	if ( keysHeld() & KEY_Y )
	{
		Debug::getInstance().drawPreviousPaths();
	}
	else
	{
		Debug::getInstance().drawPaths();
	}

	if ( end )
	{
		if ( keysDown() & KEY_START )
		{
			over = true;
		}
	}

	if ( keysDown() & KEY_A )
	{
		SoundManager::getInstance().switchMusicHearable();
	}

	if ( inputBlocked == 1 || over )
	{
		return;
	}
	else if ( keysDown() & KEY_SELECT && paused )
	{
		over = true;
	}
	else if ( keysDown() & KEY_START )
	{
		PauseAnim *panim = new PauseAnim ( paused, this );
		miscAnims.addAnimation ( panim );
		return;
	}
	else if ( !paused )
	{
		bool action = false;

		if ( keysHeld() & KEY_LEFT || gesture.isEnded() && gesture.getGesture() == GESTURE_LEFT )
		{
			moveBlock ( KEY_LEFT );
			inputBlocked = 1;
		}
		else if ( keysHeld() & KEY_RIGHT || gesture.isEnded() && gesture.getGesture() == GESTURE_RIGHT )
		{
			moveBlock ( KEY_RIGHT );
			inputBlocked = 1;
		}
		else if ( keysHeld() & KEY_UP || gesture.isEnded() && gesture.getGesture() == GESTURE_DOWN )
		{
			inputBlocked = 1;
		}
		else if ( keysHeld() & KEY_DOWN )
		{
			blockFalling();
			inputBlocked = 1;
			action = true;
		}
		if ( action )
		{
			timers.removeTimer ( TIMER_FALLING );
			timers.addTimer ( GAME_SPEED, this, TIMER_FALLING );
		}

		if ( gesture.isEnded() )
		{
			gesture.reset();
		}
	}

	if ( inputBlocked == 1 )
	{
		timers.addTimer ( GAME_INPUT_BLOCK_SPEED, this, TIMER_UNBLOCK_INPUT );
	}
}

void Game::blockFalling()
{
	moveBlock ( KEY_DOWN );
}

void Game::notify ( char *name )
{
	if ( strcmp ( TIMER_FALLING, name ) == 0 )
	{
		blockFalling();
		timers.addTimer ( GAME_SPEED, this, TIMER_FALLING );
	}
	else if ( strcmp ( TIMER_UNBLOCK_INPUT, name ) == 0 )
	{
		inputBlocked = 0;
	}
}

bool Game::isOver()
{
	return over;
}

void Game::turnPaused()
{
	consoleClear();
	paused = !paused;
}

int Game::getScore()
{
	return level.score;
}

Game::~Game()
{
}
