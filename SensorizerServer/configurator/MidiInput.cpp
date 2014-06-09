#include "MidiInput.h"
#include <Arduino.h>

#define BLE_MIDI_SERIAL_IN (Serial1)
#define USB_MIDI_SERIAL_IN (SerialUSB)

#define COMMAND_STATUS_BYTE_FLAG 0xB0

MidiInput::MidiInput(SensorizerServer* server, ConfigurationStore* store) {
	this->server = server;
	this->store = store;

	this->lastUpdatedTime = 0;
}


void MidiInput::check() {

#if IS_DRUMPANTS
	this->checkSerial(&BLE_MIDI_SERIAL_IN); 
	this->checkSerial(&USB_MIDI_SERIAL_IN); 
#endif

	// autosave if they have any pending updates
	unsigned long curTime = millis();
	if (lastUpdatedTime > 0 && curTime > lastUpdatedTime + AUTOSAVE_DELAY) {
		DEBUG_PRINT("Auto-saving configuration");
		this->saveConfiguration();
	}

}

void MidiInput::checkSerial(Stream* input) {	


	// check BLE
	// TODO: only accepts 3 byte MIDI messages!
	while (input->available() >= 3) {
		byte statChan = input->read();
		byte status = statChan & 0xF0;

		// respond to CCs
		if (status == COMMAND_STATUS_BYTE_FLAG) {
			// channel holds the sensor idx
			byte channel = statChan & 0x0F;
			byte num = input->read();
			byte val = input->read();

			updateField(channel, num, val);
		}
	}
}


inline byte MidiInput::getHandshakeResponse(byte handshakeRequest, byte value) {
	return ((int)handshakeRequest + (int)value + 420) % 127;
}

void MidiInput::sendResponse(Stream* input, ChannelCommand command, byte value1, byte value2) {
	input->write((byte)(COMMAND_STATUS_BYTE_FLAG & (byte)command));
	input->write(value1);
	input->write(value2);
}

void MidiInput::sendResponse(ChannelCommand command, byte value, byte handshakeRequest) {
	
	// craft a handshake response so the requestor knows we're legit
	byte handshakeResponse = this->getHandshakeResponse(handshakeRequest, value);

	#if IS_DRUMPANTS
		this->sendResponse(&BLE_MIDI_SERIAL_IN, command, value, handshakeResponse); 
		this->sendResponse(&USB_MIDI_SERIAL_IN, command, value, handshakeResponse); 
	#endif
}


bool MidiInput::updateField(byte sensorIdx, byte fieldIdx, byte val) {
	// channel holds the sensor idx, so only accept valid values
	if (sensorIdx < SENSOR_INPUTS_LENGTH) {
	
		if (Configurator::setField(this->server, sensorIdx, fieldIdx, val)) {
			store->setSensor(sensorIdx, fieldIdx, val);

			lastUpdatedTime = millis();
			return true;
		}
	}
	// we only save when they tell us they're done editing. don't want to burn out that eeprom!
	else if (sensorIdx == CHANNEL_COMMAND_SAVE) {
		this->saveConfiguration();

		return true;
	}
	else if (sensorIdx == CHANNEL_COMMAND_REPORT_SERIAL_NUMBER) {

		// TODO: get serial number from EEPROM!
		this->sendResponse(CHANNEL_COMMAND_REPORT_SERIAL_NUMBER, 1, val);

		return true;
	}
	else if (sensorIdx == CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION) {

		this->sendResponse(CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION, FIRMWARE_VERSION, val);

		this->sendResponse(CHANNEL_COMMAND_REPORT_BOARD_VERSION, PRESET, val);

		return true;
	}

	return false;
}


void MidiInput::saveConfiguration() {
	store->saveSensors();

	lastUpdatedTime = 0;
}

