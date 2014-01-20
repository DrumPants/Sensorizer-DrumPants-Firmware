#include "MidiRepeater.h"


MidiRepeater::MidiRepeater() {

#if IS_DRUMPANTS
	// also send to BLE
	Serial1.begin(BAUD_RATE_BLUETOOTH_LE);
#endif	
}



void MidiRepeater::onSendOutput(byte cmd, byte data1, byte data2) {
    //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
    //(sort of: http://253.ccarh.org/handout/midiprotocol/)
    //if( (cmd & 0xF0) <= 0xB0)
    bool hasSecondArg = ((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) == 0xE0);

#if IS_DRUMPANTS

	// also send to BLE
	Serial1.write(cmd);
	Serial1.write(data1);

	if (hasSecondArg)
		Serial1.write(data2);

#endif

}