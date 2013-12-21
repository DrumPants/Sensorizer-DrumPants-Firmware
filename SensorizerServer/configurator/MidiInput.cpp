#include "MidiInput.h"
#include <Arduino.h>

#define MIDI_SERIAL_IN Serial1

MidiInput::MidiInput(SensorizerServer* server) {
	this->server = server;
}


void MidiInput::check() {


#if IS_DRUMPANTS

	// check BLE
	// TODO: only accepts 3 byte MIDI messages!
	while (MIDI_SERIAL_IN.available() >= 3) {
		byte statChan = MIDI_SERIAL_IN.read();
		byte status = statChan & 0xF0;

		// respond to CCs
		if (status == 0xB0) {
			// channel holds the sensor idx
			byte channel = statChan & 0x0F;
			byte num = MIDI_SERIAL_IN.read();
			byte val = MIDI_SERIAL_IN.read();

			updateField(channel, num, val);
		}
	}

#endif

}


void MidiInput::updateField(byte sensorIdx, byte fieldIdx, byte val) {
	// channel holds the sensor idx, so only accept valid values
	if (sensorIdx < SENSOR_INPUTS_LENGTH) {

		SensorOutput* sensorInput = server->sensorInputs[sensorIdx];

		if (sensorInput != NULL) {
			float value = (float)val / 127.0;

			Configurator::setField(sensorInput, fieldIdx, value);

			// TODO: save to EEPROM
		}
	}
}
