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

#ifndef MidiInput_h
#define MidiInput_h


#include "SensorizerServer.h"
#include "Configurator.h"
#include "ConfigurationStore.h"

/***
	Time in milliseconds to wait after the last update before saving config to storage.

	This tries to save the EEPROM to write as infrequently as possible. 

	Ten seconds seems legit: long enough for them to decide to be done changing values, 
	but not too long that they might turn off the device before anything was saved.
***/
#define AUTOSAVE_DELAY (10 * 1000)

// this is sent as a channel in a status byte to indicate a command is in the second byte (like CHANNEL_COMMAND_SAVE)
#define COMMAND_STATUS_CHANNEL (0x0F)

enum ChannelCommand : byte {
	CHANNEL_COMMAND_SAVE = 0x7F,

	// these three commands report the requested number as byte1, and a valid handshake response as byte2
	// the handshake response is whatever the requestor sent as byte2, combined with the response it will send in byte1. cheapest, lamest attempt at security/DRM.
	CHANNEL_COMMAND_REPORT_BOARD_VERSION = 0x7E,
	CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION = 0x7D,
	CHANNEL_COMMAND_REPORT_SERIAL_NUMBER = 0x7C,

	CHANNEL_COMMAND_REPORT_ALL_PRESETS = 0x7B,
	CHANNEL_COMMAND_RESET_TO_DEFAULTS = 0x7A
};



class MidiInput {

	SensorizerServer* server;

	ConfigurationStore* store;

	/**
	 * System Time in milliseconds of when the last update happened.
	 *
	 * If 0, no updates have been performed.
	 */
	unsigned long lastUpdatedTime;

	/**
	 * Changes configuration on the server as well as saves to EEPROM store.
	 * @param sensorIdx Index of sensor to update, or CHANNEL_COMMAND_SAVE to save to EEPROM (other params are ignored in this case).
	 * @param fieldIdx  [description]
	 * @param value     [description]
	 * @return	true on success, false if sensorIdx doesn't exist or command not recognized.
	 */
	bool updateField(byte sensorIdx, byte fieldIdx, byte value);

	void checkSerial(Stream* input);


	/**
	 * Sends a response to a request over both BLE and USB, including the correct security response based on the securty check given by requestor.
	 * @param command          The command byte: what values are you sending? Must be 0-127.
	 * @param value            The value.
	 * @param handshakeRequest The security check given by the requestor as the second value in the packet.
	 */
	void sendResponse(ChannelCommand command, byte value, byte handshakeRequest);

	/**
	 * Sends a raw response message over the given output Stream.		
	 * @param input   Output stream
	 * @param command [description]
	 * @param value  [description]
	 */
	void sendResponse(Stream* input, ChannelCommand command, byte value);

	/**
	 * Returns a valid handshake response for the given request and value.
	 * @param  handshakeRequest [description]
	 * @param  value            [description]
	 * @return                  [description]
	 */
	byte getHandshakeResponse(byte handshakeRequest, byte value);

public:
	MidiInput(SensorizerServer* server, ConfigurationStore* store);


	/*** 
	run this every loop to check the serial RX.
	***/
	void check();

	/**
	 * Saves the current configuration to EEPROM. 
	 *
	 * This only has effect if something has been written via updateField()
	 */
	void saveConfiguration();

	/**
	 * Sends all presets and version numbers over USB. This is a blocking call!
	 */
	void sendEntireConfiguration();

	/**
	 * For testing. Only used by TestConfigurator.h
	 * @param midi the object to test.
	 */
	friend int configurator_testEEPROM(MidiInput* midi, SensorizerServer* server, ConfigurationStore* store);

	/**
	 * For testing. Only used by TestConfigurator.h
	 * @param midi the object to test.
	 */
	friend int configurator_testGetVersion(MidiInput* midi);

	friend int configurator_testSendConfig(MidiInput* midi);
};

#endif