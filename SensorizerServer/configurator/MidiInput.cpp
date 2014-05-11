#include "MidiInput.h"
#include <Arduino.h>

#define CHANNEL_COMMAND_SAVE 127

#define BLE_MIDI_SERIAL_IN (Serial1)
#define USB_MIDI_SERIAL_IN (SerialUSB)

MidiInput::MidiInput(SensorizerServer* server, ConfigurationStore* store) {
	this->server = server;
	this->store = store;
}


void MidiInput::check() {

#if IS_DRUMPANTS
	this->checkSerial(&BLE_MIDI_SERIAL_IN); 
	this->checkSerial(&USB_MIDI_SERIAL_IN); 
#endif

}

void MidiInput::checkSerial(Stream* input) {	


	// check BLE
	// TODO: only accepts 3 byte MIDI messages!
	while (input->available() >= 3) {
		byte statChan = input->read();
		byte status = statChan & 0xF0;

		// respond to CCs
		if (status == 0xB0) {
			// channel holds the sensor idx
			byte channel = statChan & 0x0F;
			byte num = input->read();
			byte val = input->read();

			updateField(channel, num, val);
		}
	}
}


bool MidiInput::updateField(byte sensorIdx, byte fieldIdx, byte val) {
	// channel holds the sensor idx, so only accept valid values
	if (sensorIdx < SENSOR_INPUTS_LENGTH) {
	
		if (Configurator::setField(this->server, sensorIdx, fieldIdx, val)) {
			store->setSensor(sensorIdx, fieldIdx, val);

			return true;
		}
	}
	// we only save when they tell us they're done editing. don't want to burn out that eeprom!
	else if (sensorIdx == CHANNEL_COMMAND_SAVE) {
		this->saveConfiguration();

		return true;
	}

	return false;
}


void MidiInput::saveConfiguration() {
	store->saveSensors();
}

