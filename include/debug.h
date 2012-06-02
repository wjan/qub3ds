#ifndef DEBUG_H
#define DEBUG_H

#include <board.h>
#include <drawer.h>

class Debug
{
	public:
		Debug();
		~Debug();
		void drawPaths();
		void drawPreviousPaths();
		void setBoard(int board[][BOARD_HEIGHT]);
        	static Debug& getInstance() {
        		static Debug instance;
			return instance;
		}
		void setEnabled(bool enabled);
		void configureDrawer(Drawer *drawer);

	private:
		void drawPathsForBoard(int board[][BOARD_HEIGHT], int width, int height);
		bool enabled;
		int b1[BOARD_WIDTH][BOARD_HEIGHT];
		int b2[BOARD_WIDTH][BOARD_HEIGHT];

};

#endif
