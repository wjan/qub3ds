#include <stylusgesture.h>
#include <stdlib.h>
#include <stdio.h>

StylusGesture::StylusGesture()
{
	reset();
}


StylusGesture::~StylusGesture()
{
}

void StylusGesture::startedAt(int x, int y) {
	startX = x;
	startY = y;
	started = true;
}

void StylusGesture::endedAt(int x, int y) {
	endX = x;
	endY = y;
	ended = true;
}

bool StylusGesture::isStarted() {
	return started;
}

bool StylusGesture::isEnded() {
	return ended;
}

void StylusGesture::reset() {
	started = false;
	ended = false;
}

int StylusGesture::getGesture() {
	int horAbs = abs(startX - endX);
	int verAbs = abs(startY - endY);
	if ( horAbs < 10 && verAbs < 10 || horAbs < verAbs && startY < endY ) {
		return GESTURE_DOWN;
	}
	else if ( horAbs > verAbs ) {
		if ( startX < endX ) {
			return GESTURE_RIGHT;
		} else if ( startX > endX ) {
			return GESTURE_LEFT;
		}
	} else {
		printf("unknown!\n");
		return GESTURE_UNKNOWN;
	}
}
