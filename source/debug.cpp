#include <debug.h>
#include <stdio.h>
#include <string.h>

Debug::Debug()
{
	memset(b1, -1, sizeof(int) * BOARD_WIDTH * BOARD_HEIGHT);
	memset(b2, -1, sizeof(int) * BOARD_WIDTH * BOARD_HEIGHT);
}

void Debug::drawPaths() {
	if (enabled) {
		drawPathsForBoard(b1, BOARD_WIDTH, BOARD_HEIGHT);
	}
}

void Debug::drawPreviousPaths() {
	if (enabled) { 
		drawPathsForBoard(b2, BOARD_WIDTH, BOARD_HEIGHT);
	}
}

void Debug::drawPathsForBoard(int board[][BOARD_HEIGHT], int width, int height) {
	for ( int i = 0; i < width; i++ )
	{
		for ( int j = 0; j < height; j++ )
		{
			if (board[i][j] >= 0) {
				printf ( "\x1b[%d;%dH%s", 22 - j, i * 3, "  ");
				printf ( "\x1b[%d;%dH%d", 22 - j, i * 3, board[i][j] );
			} else {
				printf ( "\x1b[%d;%dH%s", 22 - j, i * 3, "--" );
			}
		}
	}
}

void Debug::setBoard(int board[][BOARD_HEIGHT]) {
	if (enabled) { 
		memcpy(b2, b1, sizeof(int) * BOARD_WIDTH * BOARD_HEIGHT);
		memcpy(b1, board, sizeof(int) * BOARD_WIDTH * BOARD_HEIGHT);
	}
}

void Debug::setEnabled(bool e) {
	enabled = e;
}

void Debug::configureDrawer(Drawer *drawer) {
	if (enabled) {
		drawer->y = 4;
		drawer->z = -15;
	} else {
		drawer->y = 0;
		drawer->z = -7.5f;
	}
}

Debug::~Debug()
{
}
