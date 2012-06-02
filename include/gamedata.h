#ifndef GAMEDATA_H
#define GAMEDATA_H

class BrickColor
{
	public:
		BrickColor(float r, float g, float b);
		~BrickColor();
		float r, g, b;
};

class GameData
{
	public:
		GameData();
		~GameData();
		BrickColor *getBrickColor(int type);
	
	private:
		BrickColor *brickColors[16];
};

#endif
