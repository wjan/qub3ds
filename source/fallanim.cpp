#include <fallanim.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

FallAnim::FallAnim ( int ticks, GameData gd )
{
	ticksLeft = ticks;
	initialTicks = ticks;
	memset ( fallElements, -1, sizeof ( int ) * BOARD_WIDTH * BOARD_HEIGHT );
	gameData = gd;
}

bool FallAnim::isBlocking()
{
	return ticksLeft > BOARD_FALL_BLOCK_TIME;
}

void FallAnim::draw ( Drawer *drawer )
{
	float colorDec = ( (float)(initialTicks - ticksLeft) / (float)initialTicks );
	for ( int i = 0; i < BOARD_WIDTH; i++ )
	{
		for ( int j = 0; j < BOARD_HEIGHT; j++ )
		{
			if ( fallElements[i][j] >= 0 )
			{
				BrickColor *color = gameData.getBrickColor( fallElements[i][j] );
				drawer->drawBrick ( i, j, 0, 1, 1, color->r - colorDec, color->g - colorDec, color->b - colorDec);
			}
		}
	}
}

void FallAnim::addFallElement ( int x, int y, int type )
{
	fallElements[x][y] = type;
}

FallAnim::~FallAnim()
{
}
