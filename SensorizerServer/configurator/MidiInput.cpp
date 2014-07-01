#include "MidiInput.h"
#include <Arduino.h>

#define BLE_MIDI_SERIAL_IN (Serial1)
#define USB_MIDI_SERIAL_IN (SerialUSB)

#define COMMAND_STATUS_BYTE_FLAG (0xB0)

// start a MIDI SysEx message
#define START_SYSEX	0xF0  
// end a MIDI SysEx message
#define END_SYSEX 	0xF7  

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


byte MidiInput::getHandshakeResponse(byte handshakeRequest, byte value) {
	return ((int)handshakeRequest + (int)value + 420) % 127;
}

void MidiInput::sendResponse(Stream* input, ChannelCommand command, byte value) {
	byte statCmd = (byte)(COMMAND_STATUS_BYTE_FLAG | COMMAND_STATUS_CHANNEL);

	input->write(statCmd);
	input->write((byte)command);
	input->write(value);
}

void MidiInput::sendResponse(ChannelCommand command, byte value, byte handshakeRequest) {
	
	// craft a handshake response so the requestor knows we're legit
	byte handshakeResponse = this->getHandshakeResponse(handshakeRequest, value);

	#if IS_DRUMPANTS
		this->sendResponse(&BLE_MIDI_SERIAL_IN, command, value); 
		this->sendResponse(&USB_MIDI_SERIAL_IN, command, value); 
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
	// not a sensor update - it's a special command. fieldIdx is the ChannelCommand.
	else if (sensorIdx == COMMAND_STATUS_CHANNEL) {

		switch (fieldIdx) {
			// we only save when they tell us they're done editing. don't want to burn out that eeprom!
			case CHANNEL_COMMAND_SAVE:
				this->saveConfiguration();

				return true;
				break;

			case CHANNEL_COMMAND_REPORT_SERIAL_NUMBER:
				// TODO: get serial number from EEPROM!
				this->sendResponse(CHANNEL_COMMAND_REPORT_SERIAL_NUMBER, 1, val);
				return true;
				break;

			case CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION:
				this->sendResponse(CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION, FIRMWARE_VERSION, val);
				// fall through!

			case CHANNEL_COMMAND_REPORT_BOARD_VERSION:
				this->sendResponse(CHANNEL_COMMAND_REPORT_BOARD_VERSION, PRESET, val);

				return true;
				break;

			case CHANNEL_COMMAND_REPORT_ALL_PRESETS:
				this->sendEntireConfiguration();
				return true;
				break;
		}
	}

	return false;
}


void MidiInput::saveConfiguration() {
	store->saveSensors();

	lastUpdatedTime = 0;
}


void MidiInput::sendEntireConfiguration() {

#if IS_DRUMPANTS

DEBUG_PRINT("sendEntireConfiguration");	
	// send sysex start
	USB_MIDI_SERIAL_IN.write(START_SYSEX);

DEBUG_PRINT("SEND FIRMWARE_VERSION");
	// send version info
	this->updateField(COMMAND_STATUS_CHANNEL, CHANNEL_COMMAND_REPORT_SERIAL_NUMBER, 1);
	this->updateField(COMMAND_STATUS_CHANNEL, CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION, 1);

	// send all fields for all sensors
	for (byte sensorIdx = 0; sensorIdx < SENSOR_INPUTS_LENGTH; sensorIdx++) {

DEBUG_PRINT_NUM("SEND SENSOR CONFIG", sensorIdx);		
		for (int fieldIdx = CONFIGURATOR_FIELDS_START; fieldIdx <= CONFIGURATOR_FIELDS_END; fieldIdx++) {
DEBUG_PRINT_NUM("SEND SENSOR FIELD", fieldIdx);
			int val = Configurator::getField(this->server, sensorIdx, fieldIdx);

			if (val != CONFIGURATOR_ERROR_RETURN_CODE_FAILURE) {
				// TODO: send these via bluetooth as well?
				USB_MIDI_SERIAL_IN.write(COMMAND_STATUS_BYTE_FLAG | sensorIdx);
				USB_MIDI_SERIAL_IN.write(fieldIdx);
				USB_MIDI_SERIAL_IN.write(val);
			}
			else {
				DEBUG_PRINT_NUM("Failed to retreive config field value ", fieldIdx);
			}
		}
	}

	// end sysex
	USB_MIDI_SERIAL_IN.write(END_SYSEX);
#endif	

}
