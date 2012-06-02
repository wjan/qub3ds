#include <drawer.h>
#include <nds.h>

Drawer::Drawer()
{
	reset();
}

void Drawer::reset() {
	rx = DRAWER_RX;
	ry = DRAWER_RY;
	rz = DRAWER_RZ;
	x = DRAWER_X;
	y = DRAWER_Y;
	z = DRAWER_Z;
}

void Drawer::draw()
{
	glLoadIdentity();
	glTranslatef ( x, y, z);
	glRotatef ( rx, 1, 0, 0 );
	glRotatef ( ry, 0, 1, 0 );
	glRotatef ( rz, 0, 0, 1 );
}

void Drawer::drawBrick ( float x, float y, float z, float size, float height, float r, float g, float b )
{
	y -= 4;
	glColor3f ( r, g, b );
	float thickness = size * DRAWER_BRICK_THICKNESS;
	drawQuad ( x + thickness, y + thickness, z + thickness, size - 2 * thickness );
	drawQuad ( x + thickness, y + thickness, z + height - thickness * 2, size - 2 * thickness );
	drawLine ( x + thickness, y + thickness, z + thickness, x + thickness * 2, y + thickness, z + height - thickness * 2);
	drawLine ( x + size - thickness, y + thickness, z + thickness, x + size, y + thickness, z + height - thickness  * 2);
	drawLine ( x + thickness, y + size - thickness, z + thickness, x + thickness * 2, y + size - thickness, z + height - thickness * 2);
	drawLine ( x + size - thickness, y + size - thickness, z + thickness, x + size, y + size - thickness, z + height - thickness  * 2);
}

void Drawer::drawQuad ( float x, float y, float z, float length )
{
	float thickness = length * DRAWER_BRICK_THICKNESS;
	drawLine ( x, y, z, x + length + thickness, y + thickness, z );
	drawLine ( x + length, y, z, x + length + thickness, y + length + thickness, z );
	drawLine ( x, y + length, z, x + length, y + length + thickness, z );
	drawLine ( x, y, z, x + thickness, y + length, z );
}

void Drawer::drawLine ( float x1, float y1, float z1, float x2, float y2, float z2 )
{
	glBegin ( GL_QUADS );
	glVertex3f ( x1, y1, z1 );
	glVertex3f ( x2, y1, z1 );
	glVertex3f ( x2, y2, z2 );
	glVertex3f ( x1, y2, z2 );
	glEnd();
}

Drawer::~Drawer()
{
}
