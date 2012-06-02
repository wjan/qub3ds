#include <level.h>
#include <stdlib.h>
#include <time.h>

Level::Level()
{

	time_t unixTime = time ( NULL );
	struct tm* timeStruct = gmtime ( ( const time_t * ) &unixTime );

	int hours = timeStruct->tm_hour;
	int minutes = timeStruct->tm_min;
	int seconds = timeStruct->tm_sec;

	srand ( seconds );

	blocksAval[0].blockAvailable = 3;
	blocksAval[0].minLevel = 1;
	blocksAval[0].maxLevel = 5;

	blocksAval[1].blockAvailable = 4;
	blocksAval[1].minLevel = 6;
	blocksAval[1].maxLevel = 9;

	blocksAval[2].blockAvailable = 5;
	blocksAval[2].minLevel = 10;
	blocksAval[2].maxLevel = 14;

	blocksAval[3].blockAvailable = 6;
	blocksAval[3].minLevel = 15;
	blocksAval[3].maxLevel = 10000000;

}

void Level::reset()
{
	score = 0;
	level = 1;
	availableBlocks	= 3;
	drawNextBlocks();

}

int Level::drawNextBlocks()
{

	bl1type = rand () % availableBlocks;
	bl2type = rand () % availableBlocks;
}

void Level::increaseScore(int blocks, bool isSquare)
{
	int toAdd = 0;
	if (isSquare) {
		toAdd+= POINTS_SQUARE;
	} else {
		toAdd += blocks * POINTS_BLOCK;
	}
	if (!fallingSequenceEnded) {
		toAdd += POINTS_COMBO;
	}

	fallingSequenceEnded = false;

	score += toAdd;

	//recalculate level no
	level = ( int ) ( score / POINTS_FOR_LEVEL ) + 1;

	//recalculate available blocks
	for ( int i = 0; i < BLOCK_SW_MAX; i++ )
	{
		if ( level >= blocksAval[i].minLevel && level <= blocksAval[i].maxLevel )
		{
			availableBlocks = blocksAval[i].blockAvailable;
			break;
		}
	}
}

void Level::notifyFallingSequenceEnded() {
	fallingSequenceEnded = true;
}

Level::~Level()
{
}
