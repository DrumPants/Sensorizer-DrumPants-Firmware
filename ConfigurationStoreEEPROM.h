/***
	ConfigurationStore: saves and loads presets from EEPROM memory.
***/

#ifndef ConfigurationStoreEEPROM_h
#define ConfigurationStoreEEPROM_h

#include <Arduino.h>
#include <utility/ConfigurationStore.h>

class ConfigurationStoreEEPROM : public ConfigurationStore {

public: 
	ConfigurationStoreEEPROM();

protected:

	void write(int address, byte* data, int dataLength);
	void read(int address, byte* data, int dataLength);


};


#endif