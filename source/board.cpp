#include <board.h>
#include <debug.h>
#include <string.h>
#include <patternanim.h>
#include <fallanim.h>
#include <stdio.h>

#include <PA9.h>

Board::Board ( GameData gd )
{
	gameData = gd;
}

void Board::reset()
{
	for ( int i = 0; i < BOARD_WIDTH; i++ )
	{
		for ( int j = 0; j < BOARD_HEIGHT; j++ )
		{
			blocks[i][j].type = BLOCK_NONE;
		}
	}
	fallingSequenceEnded = true;
}

bool Board::isCollidable ( int x, int y )
{
	if ( x < 0 || x > BOARD_WIDTH - 1 || blocks[x][y].type >= 0 || y < 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool Board::isBottom ( int y )
{
	return y < 0;
}

void Board::brickSettled ( int x, int y, int z, int type )
{
	if ( z == 0 )
	{
		blocks[x][y].type = type;
	}
}

void Board::draw ( Drawer *drawer )
{
	int size = 1;
	float x = 0;
	float y = -4;
	float z = -0.1;
	glColor3f ( 1, 1, 1 );
	float rowStart;
	float colStart;
	int i, j;
	//gray bg drawing
	glColor3f ( 0.3, 0.3, 0.3 );
	drawer->drawLine ( 0, -4, -0.2, 6.1, 5.1, -0.2 );
	glColor3f ( 1, 1, 1 );
	for ( i = 0; i < BOARD_HEIGHT; i++ )
	{
		rowStart = y + i * size;
		for ( j = 0; j < BOARD_WIDTH; j++ )
		{
			colStart = x + j * size;
			drawer->drawQuad ( colStart, rowStart, z, size );
		}
	}
	for ( i = 0; i < BOARD_WIDTH; i++ )
	{
		for ( j = 0; j < BOARD_HEIGHT; j++ )
		{
			if ( blocks[i][j].type >= 0 )
			{
				BrickColor *bc = gameData.getBrickColor ( blocks[i][j].type );
				drawer->drawBrick ( i, j, 0, 1, 1, bc->r - SETTLED_BLOCK_DARKER, bc->g - SETTLED_BLOCK_DARKER, bc->b - SETTLED_BLOCK_DARKER);

			}
		}
	}
}

bool Board::findPatterns ( AnimationManager *anims )
{
	int tempBoard[BOARD_WIDTH][BOARD_HEIGHT];
	int counts[2 * BOARD_WIDTH * BOARD_HEIGHT];
	int types[2 * BOARD_WIDTH * BOARD_HEIGHT];
	memset ( tempBoard, -1, sizeof ( int ) * BOARD_WIDTH * BOARD_HEIGHT );
	memset ( counts, 0, sizeof ( int ) * 2 * BOARD_WIDTH * BOARD_HEIGHT );
	memset ( types, 0, sizeof ( int ) * 2 * BOARD_WIDTH * BOARD_HEIGHT );
	int walkNo = 0;

	//check for square pattern
	for ( int i = 0; i < BOARD_WIDTH; i++ )
	{
		for ( int j = 0; j < BOARD_HEIGHT; j++ )
		{
			if ( blocks[i][j].type > BLOCK_NONE && tempBoard[i][j] < 0 )
			{
				makeAStepForSquare ( i, j, walkNo, blocks[i][j].type, tempBoard, counts, types );
				walkNo++;
			}
		}
	}

	//check for other pattenrs
	for ( int i = 0; i < BOARD_WIDTH; i++ )
	{
		for ( int j = 0; j < BOARD_HEIGHT; j++ )
		{
			if ( blocks[i][j].type > BLOCK_NONE && tempBoard[i][j] < 0 )
			{
				makeAStep ( i, j, walkNo, blocks[i][j].type, tempBoard, counts, types );
				walkNo++;
			}
		}
	}

	bool atLeastOnePattern = false;

	for ( int walkNo = 0; walkNo < 2 * BOARD_WIDTH * BOARD_HEIGHT; walkNo++ )
	{
		//TODO: make this 4 as constant
		if ( counts[walkNo] >= 4 )
		{
			atLeastOnePattern = true;
			level->increaseScore ( counts[walkNo], types[walkNo] == TYPE_SQUARE );
		}
	}

	Debug::getInstance().setBoard ( tempBoard );

	if ( atLeastOnePattern )
	{
		PatternAnim *animation = new PatternAnim ( BOARD_PATTERN_ANIM_TIME );
		anims->addAnimation ( animation );

		for ( int i = 0; i < BOARD_WIDTH; i++ )
		{
			for ( int j = 0; j < BOARD_HEIGHT; j++ )
			{
				int walkNo = tempBoard[i][j];
				if ( walkNo >= 0 && counts[walkNo] >= 4 )
				{
					if ( types[walkNo] == TYPE_SQUARE )
					{
						//destroyed!
						blocks[i][j].type = BLOCK_NONE;
						blocks[i + 1][j].type = BLOCK_NONE;
						blocks[i][j + 1].type = BLOCK_NONE;
						blocks[i + 1][j + 1].type = BLOCK_NONE;
						animation->addPatternElement ( i, j, 2 );
						tempBoard[i][j] = BLOCK_NONE;
						tempBoard[i + 1][j] = BLOCK_NONE;
						tempBoard[i][j + 1] = BLOCK_NONE;
						tempBoard[i + 1][j + 1] = BLOCK_NONE;
					}
					else if ( types[walkNo] == TYPE_ELSE )
					{
						blocks[i][j].type = BLOCK_NONE;
						tempBoard[i][j] = BLOCK_NONE;
						animation->addPatternElement ( i, j, 1 );
					}
				}
			}
		}

	}

	return atLeastOnePattern;
}

// returns true if falling sequence is completed
void Board::letBricksFall ( AnimationManager *anims )
{
	FallAnim *fanim = new FallAnim ( BOARD_FALL_TIME, gameData );
	anims->addAnimation(fanim);

	bool completed = true;
	for ( int i = 0; i < BOARD_WIDTH; i++ )
	{
		int freeCellY = 0;
		for ( int j = 0; j < BOARD_HEIGHT; j++ )
		{
			if ( completed && blocks[i][j].type < 0 && j + 2 < BOARD_HEIGHT && blocks[i][j + 1].type < 0 && blocks[i][j + 2].type >= 0 )
			{
				// Means that we have two gaps in a row. Method can only fall for one gap so we won't have falling sequence complete. Then we'll return false. Additionally we don't want to do this checks if found at least gap (so when it's !completed)
				completed = false;
			}
			if ( blocks[i][j].type < 0 && ( j + 1 >= BOARD_HEIGHT || blocks[i][j + 1].type >= 0 ) )
			{
				freeCellY = j;
				break;
			}
		}
		int lastMovedY = 0;
		int lastMovedType = -1;
		for ( int j = 0; j < BOARD_HEIGHT; j++ )
		{
			if ( blocks[i][j].type >= 0 && j > freeCellY )
			{
				lastMovedY = j;
				lastMovedType = blocks[i][j].type;
				blocks[i][freeCellY].type = blocks[i][j].type;
				blocks[i][j].type = -1;
				freeCellY++;
			}
		}
		if ( lastMovedY > 0 )
		{
			fanim->addFallElement ( i, lastMovedY, lastMovedType );
		}
	}
	fallingSequenceEnded = completed;
}

void Board::makeAStepForSquare ( int x, int y, int walkNo, int searchType, int tempBoard[BOARD_WIDTH][BOARD_HEIGHT], int counts[BOARD_WIDTH * BOARD_HEIGHT], int types[BOARD_WIDTH * BOARD_HEIGHT] )
{
	if ( x + 1 >= BOARD_WIDTH || y + 1 >= BOARD_HEIGHT )
	{
		return;
	}
	if ( blocks[x][y].type == searchType && blocks[x + 1][y].type == searchType && blocks[x][y + 1].type == searchType && blocks[x + 1][y + 1].type == searchType )
	{
		tempBoard[x][y] = walkNo;
		tempBoard[x + 1][y] = walkNo;
		tempBoard[x][y + 1] = walkNo;
		tempBoard[x + 1][y + 1] = walkNo;
		counts[walkNo] += 4;
		types[walkNo] = TYPE_SQUARE;
	}
}

void Board::makeAStep ( int x, int y, int walkNo, int searchType, int tempBoard[BOARD_WIDTH][BOARD_HEIGHT], int counts[BOARD_WIDTH * BOARD_HEIGHT], int types[BOARD_WIDTH * BOARD_HEIGHT] )
{
	//if out of b oard
	if ( x < 0 || y < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT )
	{
		return;
	}
	//if already marked
	if ( tempBoard[x][y] >= 0 )
	{
		return;
	}

	if ( blocks[x][y].type == searchType )
	{
		tempBoard[x][y] = walkNo;
		counts[walkNo]++;
		types[walkNo] = TYPE_ELSE;
		// go four directions
		makeAStep ( x + 1, y, walkNo, searchType, tempBoard, counts, types );
		makeAStep ( x - 1, y, walkNo, searchType, tempBoard, counts, types );
		makeAStep ( x, y + 1, walkNo, searchType, tempBoard, counts, types );
		makeAStep ( x, y - 1, walkNo, searchType, tempBoard, counts, types );
	}

}


bool Board::isGameOver()
{
	return blocks[GAME_SPAWN_X][GAME_SPAWN_Y - 1].type >= 0 || blocks[GAME_SPAWN_X + 1][GAME_SPAWN_Y - 1].type > BLOCK_NONE;
}

bool Board::isOutside ( int y )
{
	return y >= BOARD_HEIGHT;
}

Board::~Board()
{
}
