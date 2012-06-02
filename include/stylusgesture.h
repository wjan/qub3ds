#ifndef STYLUSGESTURE_H
#define STYLUSGESTURE_H

#define GESTURE_LEFT 0
#define GESTURE_RIGHT 1
#define GESTURE_DOWN 2
#define GESTURE_UNKNOWN 3

class StylusGesture
{
	public:
		StylusGesture();
		~StylusGesture();
		void startedAt(int x, int y);
		void endedAt(int x, int y);
		bool isStarted();
		bool isEnded();
		void reset();
		int getGesture();
		
	private:
		bool started;
		bool ended;
		int startX, endX;
		int startY, endY;

};

#endif
