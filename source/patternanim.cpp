#include <patternanim.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

PatternAnim::PatternAnim(int ticks)
{	
	maxDelay = 0;
	initialTicks = ticks;
	ticksLeft = ticks;
	memset(elements, NULL, sizeof(PatternElement*) * BOARD_WIDTH * BOARD_HEIGHT);
	elementsNo = 0;
}

bool PatternAnim::isBlocking() {
	return true;
}

void PatternAnim::draw(Drawer *drawer) {
	for (int i = 0; i < elementsNo; i++) {
		if (elements[i] != NULL) {
			float fraction;
			if (elements[i]->delay == 0 ) {
				fraction = (float)(ticksLeft - maxDelay) / initialTicks;
			} else {
				fraction = 1;
				elements[i]->delay--;
			}
			if (fraction > 0.1) {
				drawer->drawBrick((float)elements[i]->x, (float)elements[i]->y, 0, elements[i]->size,  1, fraction, fraction, fraction);
			}
		}
	}
}

void PatternAnim::addPatternElement(int x, int y, int size ) {
	PatternElement *el = new PatternElement;
	el->x = x;
	el->y = y;
	el->size = size;
	el->delay = elementsNo * 2;
	elements[elementsNo] = el;
 	elementsNo++;
	if (el->delay > maxDelay) {
		maxDelay = el->delay;
		ticksLeft = initialTicks + maxDelay;
	}
}

PatternAnim::~PatternAnim()
{
	for (int i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++) {
		if (elements[i] != NULL) {
			delete elements[i];
		}
	}
}
