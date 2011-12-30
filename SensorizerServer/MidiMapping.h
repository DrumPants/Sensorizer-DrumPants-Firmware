#ifndef MidiMapping_h
#define MidiMapping_h

//package com.odbol.sensorizer.server;

//import com.google.gson.annotations.Expose;
//import com.odbol.sensorizer.devices.MidiDevice;

//# include "SensorizerServer.h"

#include "MidiDevice.h"
#include "OutputMapping.h"

#define VALUES_HISTORY_LENGTH 2

class MidiMapping : public OutputMapping {
	//types for msgType
	public: static const int NOTE;
	public: static const int CONTROL_CHANGE;
	public: static const int NOTE_ONE_ONLY;

	
	
	////@Expose
	private: int msgType;

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

	//@Override
	public: void init();
	

	/***
	 * adds the given values to the OSC path and sends an OSC message
	 */
	//@Override
	public: void send(double values[]);


};


#endif
