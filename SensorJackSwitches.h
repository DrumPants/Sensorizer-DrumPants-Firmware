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