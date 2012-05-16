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

#define ENABLE_DEBUG_PRINTING 0

#if ENABLE_DEBUG_PRINTING
	#define DEBUG_PRINT(text) Serial.println(text);Serial.println("-----");
	#define DEBUG_PRINT_NUM(text, num) Serial.print(text);Serial.println(num, DEC);Serial.println("-----");
#else
	#define DEBUG_PRINT(text)   
	#define DEBUG_PRINT_NUM(text, num)   
#endif

class SensorizerServer {

private: 

	MidiDevice* midiDevice;

public:
	
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


