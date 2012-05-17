#ifndef SensorizerServer_h
#define SensorizerServer_h

#include <Arduino.h>
/*
# include <HardwareSerial.h>
# include <WConstants.h> 
*/

#include "MidiDevice.h"
#include "SensorOutput.h"

#include "Debug.h"

#define SENSOR_INPUTS_LENGTH 6


class SensorizerServer {

private: 


public:
	//should maybe be private but I don't believe in writing everything twice.
	MidiDevice* midiDevice;
	
	
	static const double SENSOR_VALUE_NULL = -1.0;

	SensorOutput* sensorInputs[SENSOR_INPUTS_LENGTH];
	
	SensorizerServer();
	~SensorizerServer();
	
	void init();
	
	//reads and sends a single pin
	void readPin(int pinIdx, int pinValue);
	
	void loadPreset();
	
};

#endif


