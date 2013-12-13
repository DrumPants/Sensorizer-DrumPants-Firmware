#include "Metro.h"
#include <Arduino.h>

Metro::Metro() {
	lastTimeTicked = millis();
	timePerTick = 1000;
}
/***

Returns > 0 if a tick has happened.

Returns 0 if between ticks.

***/
int Metro::hasTicked() {
	int curTime = millis();
	int timeEllapsed = curTime - this->lastTimeTicked;

	// TODO: if time per tick has not ellapsed from last time, return.
	if (timeEllapsed < timePerTick)
		return 0;


	this->lastTimeTicked = curTime;

	return curTime;
}