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

//#include "general_midi_sounds.h"

#define ENABLE_SEND_METRONOME_TO_USB_AND_BLE 1

#define METRONOME_MIDI_CHANNEL 10
// set to MetronomeClick
#define METRONOME_NOTE 33

// play quarter notes
#define METRONOME_TIME_DIVSION (1/4)

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

	Metro metronome;

	// call every tick()
	void metronomeTick();
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

	/**
	 * Starts the system metronome at the given BPM.
	 *
	 * You must call tick() regularly for this to work.
	 * @param bpm Beats per minute
	 * @param divisions 
	 */
	void startMetronome(int bpm, int divisions = 4);
	void stopMetronome();

	/**
	 * Returns current BPM of the metronome, or 0 if it's disabled.
	 * @return [description]
	 */
	int getMetronomeBPM(int divisions = 4);
	
	/****
		Returns the sensor index that is sending the given note for the curretnly selected scale.
	***/
	int getSensorIdxForNote(byte note);
};

#endif


