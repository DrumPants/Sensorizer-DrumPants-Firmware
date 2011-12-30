#ifndef SensorizerServer_h
#define SensorizerServer_h

#include <Arduino.h>
/*
# include <HardwareSerial.h>
# include <WConstants.h> 
*/

#include "MidiDevice.h"
#include "SensorOutput.h"

#define SENSOR_INPUTS_LENGTH 6

class SensorizerServer {

private: 

	MidiDevice* midiDevice;

public:
	
	static const double SENSOR_VALUE_NULL = -1.0;

	SensorOutput* sensorInputs[SENSOR_INPUTS_LENGTH];
	
	SensorizerServer();
	~SensorizerServer();
	
	void loadPreset();
	
};

#endif


