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

/***
	ConfigurationStore: saves and loads presets from EEPROM memory.
***/

#ifndef ConfigurationStoreEEPROM_h
#define ConfigurationStoreEEPROM_h

#include <Arduino.h>
#include <utility/ConfigurationStore.h>
#include <SensorizerServer.h>

#include <I2C_eeprom.h>

// it's only 1Kbit!!!
#define EEPROM_SIZE_IN_BYTES EEPROM_SIZE_IN_KBITS * 1024 / 8

// the address of your EEPROM
#define DEVICEADDRESS (0x50)


class ConfigurationStoreEEPROM : public ConfigurationStore {

public: 
	ConfigurationStoreEEPROM(SensorizerServer* server);

	/**
	 * Wipes the configuration memory and resets to factory defaults.
	 */
	void eraseAllAndReset();
protected:

	void write(unsigned int address, byte* data, int dataLength);
	void read(unsigned int address, byte* data, int dataLength);

private:

	I2C_eeprom eeprom;
};


#endif