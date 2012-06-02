#ifndef LEVEL_H
#define LEVEL_H

#include <parameters.h>

//number of ranges (BlocksAvailability instances - blocksAval)
#define BLOCK_SW_MAX 4

class BlocksAvailability {
	public:
		int minLevel;
		int maxLevel;
		int blockAvailable;
};

class Level
{
	public:
		Level();
		~Level();
		int level;
		int score;
		int drawNextBlocks();
		int bl1type;
		int bl2type;
		void increaseScore(int blocks, bool isSquare);
		void notifyFallingSequenceEnded();
		int availableBlocks;
		BlocksAvailability blocksAval[BLOCK_SW_MAX];
		void reset();

	private:
		bool fallingSequenceEnded;
};

#endif
