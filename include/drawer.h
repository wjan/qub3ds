#ifndef DRAWER_H
#define DRAWER_H

#include <parameters.h>

class Drawer
{
	public:
		Drawer();
		~Drawer();
		void reset();
		void draw();
		void drawBrick(float x, float y, float z, float size, float height, float r, float g, float b);
		void drawQuad(float x, float y, float z, float length);
		void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
		float rx, ry, rz, x, y, z;
};

#endif
