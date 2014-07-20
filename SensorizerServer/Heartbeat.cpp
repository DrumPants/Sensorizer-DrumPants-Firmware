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
	SerialUSB.write(data);
#endif	
}