#include <nds.h>
#include <stdio.h>
#include <blockpair.h>
#include <brickdisanim.h>
#include <soundmanager.h>


BlockPair::BlockPair ( GameData gd )
{
	gameData = gd;
}

void BlockPair::spawn ( int x, int y, int type1, int type2 )
{
	bl1.spawn ( x, y, type1, true );
	bl2.spawn ( x + 1, y, type2, false );
}

bool BlockPair::moveSingle ( int dir, Board *board )
{
	bool mooved = false;
	Block *bl = &bl1;
	if ( !bl->active )
	{
		bl = &bl2;
	}
	if ( dir == KEY_DOWN )
	{
		bottomCollision ( board );
		bl->posY--;
		mooved = true;
	}
	return mooved;
}

bool BlockPair::isBlockUseless ( Block *bl1, Block *bl2, Board *board )
{
	if ( !bl1->active && bl2->active && bl2->posZ == 1 )
	{
		// TODO: mark this brick as animation-destroyed
		BrickDisAnim *anim = new BrickDisAnim ( bl2->posX, bl2->posY, bl2->posZ, BPAIR_DESTROY_TIME );
		anims->addAnimation ( anim );
		bl2->active = false;
		SoundManager::getInstance().blockDisposed();
		return true;
	}
	else
	{
		return false;
	}
}

bool BlockPair::move ( int dir, Board *board )
{
	bool mooved = false;
	bool justSpawned = board->isOutside ( bl1.posY );
	if ( !isBlockUseless ( &bl1, &bl2, board ) )
	{
		isBlockUseless ( &bl2, &bl1, board );
	}

	if ( bl1.active && bl2.active )
	{
		if ( !justSpawned &&  dir == KEY_LEFT )
		{
			if ( bl1.position == BLOCK_TOP )
			{
				// when block 1 is on top
				if ( !board->isCollidable ( bl1.posX - 1, bl1.posY ) )
				{
					bl1.posX--;
					bl1.posZ--;
					bl1.position = BLOCK_LEFT;
					bl2.position = BLOCK_RIGHT;
					mooved = true;
				}
			}
			else if ( bl2.position == BLOCK_TOP )
			{
				// when block 2 is on top
				if ( !board->isCollidable ( bl2.posX - 1, bl2.posY ) )
				{
					bl2.posX--;
					bl2.posZ--;
					bl2.position = BLOCK_LEFT;
					bl1.position = BLOCK_RIGHT;
					mooved = true;
				}
			}
			else if ( bl1.posX < bl2.posX )
			{
				// blocks are horizontal, 1st on left
				bl2.posX--;
				bl2.posZ++;
				bl2.position = BLOCK_TOP;
				mooved = true;
			}
			else if ( bl1.posX > bl2.posX )
			{
				// blocks are horizontal, 2nd on left
				bl1.posX--;
				bl1.posZ++;
				bl1.position = BLOCK_TOP;
				mooved = true;
			}
			else
			{
				// when bricks are vertical
				if ( bl1.posY < bl2.posY )
				{
					if ( !board->isCollidable ( bl1.posX - 1, bl1.posY ) )
					{
						bl1.posX--;
						bl2.posX--;
						mooved = true;
					}
				}
				else
				{
					if ( !board->isCollidable ( bl2.posX - 1, bl2.posY ) )
					{
						bl1.posX--;
						bl2.posX--;
						mooved = true;
					}
				}
			}
		}
		else if ( !justSpawned && dir == KEY_RIGHT )
		{
			if ( bl1.position == BLOCK_TOP )
			{
				if ( !board->isCollidable ( bl1.posX + 1, bl1.posY ) )
				{
					bl1.posX++;
					bl1.posZ--;
					bl1.position = BLOCK_RIGHT;
					bl2.position = BLOCK_LEFT;
					mooved = true;
				}
			}
			else if ( bl2.position == BLOCK_TOP )
			{
				if ( !board->isCollidable ( bl2.posX + 1, bl2.posY ) )
				{
					bl2.posX++;
					bl2.posZ--;
					bl2.position = BLOCK_RIGHT;
					bl1.position = BLOCK_LEFT;
					mooved = true;
				}
			}
			else if ( bl1.posX < bl2.posX )
			{
				bl1.posX++;
				bl1.posZ++;
				bl1.position = BLOCK_TOP;
				mooved = true;
			}
			else if ( bl1.posX > bl2.posX )
			{
				bl2.posX++;
				bl2.posZ++;
				bl2.position = BLOCK_TOP;
				mooved = true;
			}
			else
			{
				// when bricks are vertical
				if ( bl1.posY < bl2.posY )
				{
					if ( !board->isCollidable ( bl1.posX + 1, bl1.posY ) )
					{
						bl1.posX++;
						bl2.posX++;
						mooved = true;
					}
				}
				else
				{
					if ( !board->isCollidable ( bl2.posX + 1, bl2.posY ) )
					{
						bl1.posX++;
						bl2.posX++;
						mooved = true;
					}
				}
			}
		}
		else if ( dir == KEY_DOWN )
		{
			bottomCollision ( board );
			if ( bl1.position == BLOCK_TOP ) // and no collision
			{
				bl1.posY--;
				bl1.posZ--;
				bl1.position = BLOCK_DOWN;
				bl2.position = BLOCK_UP;
			}
			else if ( bl2.position == BLOCK_TOP )
			{
				bl2.posY--;
				bl2.posZ--;
				bl2.position = BLOCK_DOWN;
				bl1.position = BLOCK_UP;
			}
			else if ( bl1.posX == bl2.posX && bl1.posY > bl2.posY )
			{
				bl1.posY--;
				bl1.posZ++;
				bl1.position = BLOCK_TOP;
			}
			else if ( bl1.posX == bl2.posX && bl1.posY < bl2.posY )
			{
				bl2.posY--;
				bl2.posZ++;
				bl2.position = BLOCK_TOP;
			}
			else
			{
				bl1.posY--;
				bl2.posY--;
			}
			mooved = true;
		}
	}
	else
	{
		mooved = moveSingle ( dir, board );
	}
	return mooved;
}

void BlockPair::bottomCollision ( Board *board )
{


	if ( bl1.active && bl2.active && bl1.posX == bl2.posX && bl1.posY != bl2.posY &&
	        ( bl2.posY < bl1.posY && board->isCollidable ( bl2.posX, bl2.posY - 1 )
	          ||
	          bl1.posY < bl2.posY && board->isCollidable ( bl1.posX, bl1.posY - 1 ) )
	   )
	{
		// when 2 vertical blocks hit ground
		board->brickSettled ( bl1.posX, bl1.posY, bl1.posZ, bl1.type );
		board->brickSettled ( bl2.posX, bl2.posY, bl2.posZ, bl2.type );
		bl1.active = false;
		bl2.active = false;
	}

	bool delay = false;

	if ( bl1.posZ == 0 && bl1.active && board->isCollidable ( bl1.posX, bl1.posY - 1 ) )
	{
		// when bl1 makes collision
		delay = true;
	}
	if ( bl2.posZ == 0 && bl2.active && board->isCollidable ( bl2.posX, bl2.posY - 1 ) )
	{
		// when bl2 makes collision
		board->brickSettled ( bl2.posX, bl2.posY, bl2.posZ, bl2.type );
		bl2.active = false;
	}

	if ( delay )
	{
		board->brickSettled ( bl1.posX, bl1.posY, bl1.posZ, bl1.type );
		bl1.active = false;
	}

	if ( !isBlockUseless ( &bl1, &bl2, board ) )
	{
		isBlockUseless ( &bl2, &bl1, board );
	}
}

bool BlockPair::isSettled()
{
	return !bl1.active && !bl2.active;
}


float BlockPair::getLighterColor(float color) {
	return color;
	if (color < 1) {
		return color + ACTIVE_BLOCK_LIGHTER;
	}
}

void BlockPair::draw ( Drawer *drawer )
{
	if ( bl1.active )
	{
		BrickColor *bc = gameData.getBrickColor ( bl1.type );
		drawer->drawBrick ( bl1.posX, bl1.posY, bl1.posZ, 1, 1, getLighterColor(bc->r), getLighterColor(bc->g), getLighterColor(bc->b) );
	}
	if ( bl2.active )
	{
		BrickColor *bc = gameData.getBrickColor ( bl2.type );
		drawer->drawBrick ( bl2.posX, bl2.posY, bl2.posZ, 1, 1, getLighterColor(bc->r), getLighterColor(bc->g), getLighterColor(bc->b) );
	}
}

BlockPair::~BlockPair()
{
}
