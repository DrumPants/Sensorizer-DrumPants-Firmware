/**
 * Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
	if (timePerTick == 0) return 0;
	
	int curTime = millis();
	int timeEllapsed = curTime - this->lastTimeTicked;

	// TODO: if time per tick has not ellapsed from last time, return.
	if (timeEllapsed < timePerTick)
		return 0;


	this->lastTimeTicked = curTime;

	return curTime;
}