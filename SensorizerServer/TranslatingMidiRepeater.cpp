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

#include "TranslatingMidiRepeater.h"

#include "SensorizerServer.h"


TranslatingMidiRepeater::TranslatingMidiRepeater(SensorizerServer* server) {
	MidiRepeater();

	this->server = server;
}



void TranslatingMidiRepeater::onSendOutput(byte cmd, byte data1, byte data2) {
    int idx;
    if (ENABLE_SEND_METRONOME_TO_USB_AND_BLE &&
    	((cmd & 0x0F) == METRONOME_MIDI_CHANNEL)) { // allow metronome to send (for testing)

    	// prepare for USB to get it.
    	idx = 61; // send a phony index just so it will sound in the app.
    	cmd = (cmd & 0xF0) | MIDI_CHANNEL;
    } 
    else {  
    	idx = server->getSensorIdxForNote(data1);      
    }
    
    // send BLE the true MIDI note, not the idx
    sendTo(true, cmd, data1, data2);

    if (idx != -1) {
    	// send a 0 note. sure, why not? it's creepy, but whatevs
	    sendTo(false, cmd, idx, data2);
	}
	else {
		DEBUG_PRINT("Failed to find sensor index");
	}
}