#include <gamedata.h>
#include <stdlib.h>

BrickColor::BrickColor(float _r, float _g, float _b) {
	r = _r;
	g = _g;
	b = _b;
}

BrickColor::~BrickColor() {
}

GameData::GameData()
{
	brickColors[0] = new BrickColor(1, 0, 0);
	brickColors[1] = new BrickColor(0, 1, 0);
	brickColors[2] = new BrickColor(0, 0, 1);
	brickColors[3] = new BrickColor(1, 1, 0);
	brickColors[4] = new BrickColor(1, 0, 1);
	brickColors[5] = new BrickColor(1, 0.5, 0);
}


BrickColor* GameData::getBrickColor(int type) {
	return brickColors[type];
}

GameData::~GameData()
{
}
