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

#include "MidiRepeater.h"


MidiRepeater::MidiRepeater() {

//#if IS_DRUMPANTS
	// also send to BLE
	Serial1.begin(BAUD_RATE_BLUETOOTH_LE);
//#endif	


#if ENABLE_USB_MIDI

#endif
}



void MidiRepeater::onSendOutput(byte cmd, byte data1, byte data2) {
	sendTo(true, cmd, data1, data2);
	sendTo(false, cmd, data1, data2);
}

void MidiRepeater::sendTo(bool isBle, byte cmd, byte data1, byte data2) {

    //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
    //(sort of: http://253.ccarh.org/handout/midiprotocol/)
    //if( (cmd & 0xF0) <= 0xB0)
    bool hasSecondArg = ((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) == 0xE0);

	//DEBUG_PRINT_NUM("Send to BLE: ", cmd);
//#if IS_DRUMPANTS

    if (isBle) {
		// also send to BLE
		Serial1.write(cmd);
		Serial1.write(data1);

		if (hasSecondArg) {
			Serial1.write(data2);
		}
    }

#if ENABLE_SENDING_MIDI_OVER_USB    
    else {

#	if ENABLE_USB_MIDI
		// support for 32-byte class-compliant USB MIDI packets. (DrumPants 2.0)
	    byte status = (cmd & 0xF0);
	    byte header = (status >> 4) & 0x0F;

		// class-compliant USB MIDI packets must ALWAYS be 32-bytes, zero padded. so we send data2 no matter what.
		uint8_t midiPacket[] = {header, cmd, data1, data2};
    	MidiUSB.write(midiPacket, 4);

#	endif    	
    	
		// also send to USB, normal Serial style. (for DrumPants 1.0 app)
		SerialUSB.write(cmd);
		SerialUSB.write(data1);

		if (hasSecondArg) {
			SerialUSB.write(data2);
		}
	
	}
#endif

//#endif

}


