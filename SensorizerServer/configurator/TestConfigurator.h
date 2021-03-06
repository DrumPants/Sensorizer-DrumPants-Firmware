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

#ifndef TEST_CONFIGURATOR_H
#define TEST_CONFIGURATOR_H

#include "Constants.h"
#include "Debug.h"

#include "SensorizerServer.h"
#include "MidiInput.h"
#include "Configurator.h"

#define NOTE_TEST_VALUE 100


bool configurator_checkMapping(SensorizerServer* server, byte sensorIdx) {
	MidiMapping* mapping = server->sensorInputs[sensorIdx]->getMidiMapping(0);
	if (mapping != NULL) {
		if (mapping->note == NOTE_TEST_VALUE) {
			return true;
		}
	}

	return false;
}

/**
 * Tests configuring sensors and writing to EEPROM.
 * @param  midi   [description]
 * @param  server [description]
 * @return        0 on success, number of errors otherwise
 */
int configurator_testEEPROM(MidiInput* midi, SensorizerServer* server, ConfigurationStore* store) {
	int numErrors = 0;

	byte sensorIdx = 1;
	byte fieldIdx = 0;


	DEBUG_PRINT("Testing Configurator: note update");

	if (!midi->updateField(sensorIdx, MIDIMAPPING_NOTE, NOTE_TEST_VALUE)) {
		numErrors++;
	}

	midi->saveConfiguration();

	if (!configurator_checkMapping(server, sensorIdx)) {
		numErrors++;
		DEBUG_PRINT("Configurator set failed: note not updated");
	}




	DEBUG_PRINT("Testing Configurator: loading from storage");
	
	// change to something else in memory only (don't save), so we can load over it
	if (!midi->updateField(sensorIdx, MIDIMAPPING_NOTE, 60)) {
		numErrors++;
	}

	store->loadSensors();

	if (!configurator_checkMapping(server, sensorIdx)) {
		numErrors++;
		DEBUG_PRINT("Configurator load failed: note not loaded");
	}

	DEBUG_PRINT("NOTE: Configurator autosave should happen in 10 seconds.");

	return numErrors;
}


/**
 * Tests configuring sensors and writing to EEPROM.
 * @param  midi   [description]
 * @return        0 on success, number of errors otherwise
 */
int configurator_testGetVersion(MidiInput* midi) {
	int numErrors = 0;

	byte handshake = rand() * 127;
	byte boardHandshakeResponse = midi->getHandshakeResponse(handshake, PRESET);
	byte handshakeResponse = midi->getHandshakeResponse(handshake, FIRMWARE_VERSION);

	DEBUG_PRINT_NUM("Requesting firmware version with handshake:", handshake);

	if (!midi->updateField(COMMAND_STATUS_CHANNEL, CHANNEL_COMMAND_REPORT_FIRMWARE_VERSION, handshake)) {
		numErrors++;
	}

	DEBUG_PRINT_NUM("Firmware Handshake response should be:", handshakeResponse);
	DEBUG_PRINT_NUM("Board Handshake response should be:", boardHandshakeResponse);


	DEBUG_PRINT_NUM("Requesting serial number with handshake:", handshake);

	if (!midi->updateField(COMMAND_STATUS_CHANNEL, CHANNEL_COMMAND_REPORT_SERIAL_NUMBER, handshake)) {
		numErrors++;
	}

	return numErrors;
}


int configurator_testSendConfig(MidiInput* midi) {
	int numErrors = 0;
	//midi->sendEntireConfiguration();

	if (!midi->updateField(COMMAND_STATUS_CHANNEL | CHANNEL_COMMAND_REPORT_ALL_PRESETS, CHANNEL_COMMAND_REPORT_ALL_PRESETS, 1)) {
		numErrors++;
	}

	return numErrors;
}

#endif