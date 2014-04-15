#ifndef SensorizerServer_h
#define SensorizerServer_h

#include <Arduino.h>
/*
# include <HardwareSerial.h>
# include <WConstants.h> 
*/
#include "Constants.h" 

#include "MidiDevice.h" 
#include "SensorOutput.h"

#include "looper/EventLooper.h"

#include "Metro.h"

#include "Debug.h"

#include "TranslatingMidiRepeater.h"


#define SENSOR_INPUTS_LENGTH ((ANALOG_PIN_END + 1) - ANALOG_PIN_START)

#if SENSOR_INPUTS_LENGTH > NOTE_PRESETS_ELEMENT_LENGTH
  #warning "TODO: update note preset arrays for more than 6 sensors"
#endif


#define TIME_PER_TICK (LOOP_LENGTH_IN_SECONDS * 1000 / LOOP_LENGTH_IN_TICKS)

class SensorizerServer {

private: 
	
#if ENABLE_LOOPER
	EventLooper* looper;
#endif

	Metro metro;
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
	
	//takes a 6 element array of notes and loads them into the MIDI preset
	void loadNotes(byte notes[]);

	void tick();
	
	/****
		Returns the sensor index that is sending the given note for the curretnly selected scale.
	***/
	int getSensorIdxForNote(byte note);
};

#endif


