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

protected:

	void write(unsigned int address, byte* data, int dataLength);
	void read(unsigned int address, byte* data, int dataLength);

private:

	I2C_eeprom eeprom;
};


#endif