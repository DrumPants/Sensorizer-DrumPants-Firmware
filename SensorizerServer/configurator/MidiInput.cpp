#include "MidiInput.h"
#include <Arduino.h>

#define MIDI_SERIAL_IN Serial

MidiInput::MidiInput(SensorizerServer* server) {
	this->server = server;
}


void MidiInput::check() {


#if IS_DRUMPANTS

	// check BLE
	// TODO: only accepts 3 byte MIDI messages!
	while (MIDI_SERIAL_IN.available() >= 3) {
		byte status = MIDI_SERIAL_IN.read();

		// respond to CCs
		if (status == 0xB0) {
			byte num = MIDI_SERIAL_IN.read();
			byte val = MIDI_SERIAL_IN.read();

			if (num < SENSOR_INPUTS_LENGTH) {
				SensorOutput* sensorInput = server->sensorInputs[num];

				if (sensorInput != NULL) {
					float value = (float)val / 127.0;
					sensorInput->cutoffRange.low = value;

					// TODO: save to EEPROM
				}
			}
		}
	}

#endif

}
