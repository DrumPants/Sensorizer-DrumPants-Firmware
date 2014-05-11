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


	return numErrors;
}

#endif