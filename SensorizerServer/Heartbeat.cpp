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

#include "Heartbeat.h"

#define HEARTBEAT_START 0xF8 

Heartbeat::Heartbeat() {
	this->metro.timePerTick = 1000; // only send every one second so they don't disconnect us from USB
}

void Heartbeat::tick() {

	if (this->metro.hasTicked()) {
		this->writeToSerial(HEARTBEAT_START);
	}
}


inline void Heartbeat::writeToSerial(byte data) {

#if ENABLE_SENDING_MIDI_OVER_USB
// we still send the heartbeat over Serial, so the DrumPants app can use Serial instead of USB.
// 
// #	if ENABLE_USB_MIDI

// 	// TODO, if needed? 
// 	// since the OS will now alert us when a MIDI device is disconnected, 
// 	// we don't need to send a heartbeat to let the DrumPants app know to not disconnect us.

// #	else

	SerialUSB.write(data);

// #	endif	
#endif	

}