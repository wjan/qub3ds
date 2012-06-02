#include <animationmanager.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

AnimationManager::AnimationManager()
{
	used = 0;
	memset ( animations, NULL, sizeof ( Animation* ) * ANIM_MAX );
}

void AnimationManager::draw ( Drawer *drawer )
{
	for ( int i = 0; i < ANIM_MAX; i++ )
	{
		if ( animations[i] != NULL )
		{
			animations[i]->draw ( drawer );
		}
	}
}

void AnimationManager::tick()
{
	for ( int i = 0; i < ANIM_MAX; i++ )
	{
		if ( animations[i] != NULL )
		{
			if ( animations[i]->anyTicksLeft() )
			{
				animations[i]->tick();
			}
			else
			{
				animations[i]->performEndAction();
				delete animations[i];
				animations[i] = NULL;
				if ( i < used - 1 )
				{
					animations[i] = animations[used - 1];
					animations[used - 1] = NULL;
				}
				used--;
			}
		}
	}
}

void AnimationManager::addAnimation ( Animation *animation )
{
	if ( used < ANIM_MAX )
	{
		animations[used] = animation;
		used++;
	}
	else
	{
		printf ( "WARNING: no free slot for animation\n" );
	}
}

bool AnimationManager::isBlocking()
{
	for ( int i = 0; i < used; i++ )
	{
		if ( animations[i]->isBlocking() )
		{
			return true;
		}
	}
	return false;
}

AnimationManager::~AnimationManager()
{
}
