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

#ifndef SENSORJACKSWITCHES_h
#define SENSORJACKSWITCHES_h


/**
 * Call once at the beginning.
 */
void initSensorJackSwitches();

/**
 * 
	Checks the switch on a sensor jack to determine if a sensor is actually plugged in.
	Use for disabling inputs that aren't attached to something, to avoid floating input data.
 * @param  analogPinIdx The index of the analog pin the sensor is connected to. The correct switch will be checked for that jack.
 * @return              True if sensor is connected, false if nothing plugged into that jack.
 */
bool isSensorPluggedIn(int analogPinIdx);


#endif