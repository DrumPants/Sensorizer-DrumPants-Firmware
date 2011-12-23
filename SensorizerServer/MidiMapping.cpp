#include "MidiMapping.h"
//package com.odbol.sensorizer.server;

//import com.google.gson.annotations.Expose;
//import com.odbol.sensorizer.devices.MidiDevice;

#include "OutputMapping.h"

	//types for msgType
	const int MidiMapping::NOTE = 0;
	const int MidiMapping::CONTROL_CHANGE = 1;
	const int MidiMapping::NOTE_ONE_ONLY = 3;

	
	
	////@Expose
	//int MidiMapping::msgType;

	int MidiMapping::getMsgType() {
		return this->msgType;
	}

	void MidiMapping::setMsgType(int msgType) {
		this->msgType = msgType;
		this->isNoteOn = false; //reset???
	}

/*
	//these are strings so you can do fancy things like "${0}" for variable substitution
	////@Expose
	string MidiMapping::channel;
	
	////@Expose
	string MidiMapping::note;
	
	////@Expose
	string MidiMapping::duration;
	
	////@Expose
	string MidiMapping::midiDeviceName;
	

	
	bool MidiMapping::isNoteOn;

	//VALUES_HISTORY_LENGTH is the threshold for detecting changes from NULL to not NULL
	//TODO: add adjustable threshold
	double MidiMapping::valuesHistory[2][1] = {NULL, NULL};
*/		
	
	MidiMapping::MidiMapping(SensorizerServer* midiDevice) {
		this->msgType = MidiMapping::NOTE_ONE_ONLY;
		this->channel = 1;
		this->note = 60;
		this->duration = 127;
		this->midiDeviceName = "";
		this->isNoteOn = false;
		
		this->midiDevice = midiDevice;
	}

	//@Override
	void MidiMapping::init() {
		
	}
	

	/***
	 * adds the given values to the OSC path and sends an OSC message
	 */
	//@Override
	void MidiMapping::send(double values[]) {
		//if (midiDevice != NULL) {
			//throw new Exception();
			
			if (msgType != NOTE_ONE_ONLY && values[0] == SensorizerServer::SENSOR_VALUE_NULL)
				return;
			
			//TODO: move this parsing to setter methods to optimize
			int c = channel; //atoi(channel);
			int n = note; //atoi(note);
			
			int vel = (int)values[0] * 127.0; //truncate is GOOD ENOUGH Math.round(values[0] * 127.0);

			switch (msgType) {
			case MidiMapping::NOTE:
				//int dur = duration; //Integer.parseInt(duration);
				midiDevice->note(c, n, vel, duration);
				break;
			case MidiMapping::CONTROL_CHANGE:
				//TODO
				//midiDevice->cc(c, n, vel);
				break;
			case MidiMapping::NOTE_ONE_ONLY:
				int numNULLs = 0;
				for (int i = 0; i < VALUES_HISTORY_LENGTH; i++) {
					if (valuesHistory[i] != NULL) {
						if (valuesHistory[i][0] == SensorizerServer::SENSOR_VALUE_NULL) {
							numNULLs++;
						}
					}
					else 
						numNULLs -= (VALUES_HISTORY_LENGTH + 2); //make sure any NULLs (history just started) don't trigger note on
				}
					
				if (values[0] != SensorizerServer::SENSOR_VALUE_NULL && numNULLs == VALUES_HISTORY_LENGTH) { //been NULLs the whole time of history threshold. turn off!
					//note on
					//if (!isNoteOn) {
						//dur = Integer.parseInt(duration);
						midiDevice->note(true, c, n, duration); //duration is now velocity

					//	isNoteOn = true;
					//}
				}
				else if (values[0] == SensorizerServer::SENSOR_VALUE_NULL && numNULLs == 0) { //value now NULL, only note off when it was on long enough for the threshold 
					//note off
					//if (isNoteOn) {
						midiDevice->note(false, c, n, 0);
						
					//	isNoteOn = false;
					//}
				}
				
				break;
			}

			//record history for threshold
			*valuesHistory[1] = *valuesHistory[0];
			*valuesHistory[0] = *values;
		//}
		
	}

	
