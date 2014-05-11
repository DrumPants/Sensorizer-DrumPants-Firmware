/***
	ConfigurationStore: saves and loads presets from EEPROM memory.
***/

#ifndef ConfigurationStoreEEPROM_h
#define ConfigurationStoreEEPROM_h

#include <Arduino.h>
#include <utility/ConfigurationStore.h>
#include <SensorizerServer.h>

class ConfigurationStoreEEPROM : public ConfigurationStore {

public: 
	ConfigurationStoreEEPROM(SensorizerServer* server);

protected:

	void write(unsigned int address, byte* data, int dataLength);
	void read(unsigned int address, byte* data, int dataLength);


};


#endif