#ifndef MidiMapping_h
#define MidiMapping_h

//package com.odbol.sensorizer.server;

//import com.google.gson.annotations.Expose;
//import com.odbol.sensorizer.devices.MidiDevice;

//# include "SensorizerServer.h"

#include "MidiDevice.h"
#include "OutputMapping.h"
#include "PendingNoteQueue.h" 
#include "Metro.h"

#define VALUES_HISTORY_LENGTH 2

// the num of milliseconds between sending CC notes, so we don't overload the pipes.
#define CC_MESSAGE_THROTTLE_INTERVAL 20

class MidiMapping : public OutputMapping {
	//types for msgType
	public: static const int NOTE;
	public: static const int CONTROL_CHANGE;
	public: static const int NOTE_ONE_ONLY;
	public: static const int PITCH_BEND;

	// keeps track of when the last CC was sent so we don't overload things
	private: Metro metro;
	
	// holds every note we sent so we can send a note off later.
	private: PendingNoteQueue* pendingNoteOffs;
	
	////@Expose
	public: int msgType;

	public: int getMsgType();

	public: void setMsgType(int msgType);

	//these are strings so you can do fancy things like "${0}" for variable substitution
	////@Expose
	public: int channel;
	
	////@Expose
	public: int note;
	
	////@Expose
	public: int duration;
	
	////@Expose
	//public: string midiDeviceName;
	
	
	private: bool isNoteOn;
	
	//valuesHistory.length is the threshold for detecting changes from null to not null
	//TODO: add adjustable threshold
	private: double valuesHistory[VALUES_HISTORY_LENGTH][1];
		
	public: MidiDevice* midiDevice;
	
	public: MidiMapping(MidiDevice* midiDevice);

	~MidiMapping();

	//@Override
	public: void init();
	
	void tick();

	/***
	 * adds the given values to the OSC path and sends an OSC message
	 */
	//@Override
	public: void send(double values[]);


};


#endif
