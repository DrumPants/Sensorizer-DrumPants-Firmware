#include "ConfigurationStoreEEPROM.h"

#include "Debug.h"
#include <SensorizerServer.h>

#define TEMPBUFFSIZE 32

ConfigurationStoreEEPROM::ConfigurationStoreEEPROM(SensorizerServer* s)  : ConfigurationStore(s),  eeprom(DEVICEADDRESS, EEPROM_SIZE_IN_BYTES) {
	eeprom.begin();
}

void ConfigurationStoreEEPROM::write(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS("Writing to EEPROM ", address, dataLength);

    eeprom.writeBlock(address, data, dataLength);
}

void ConfigurationStoreEEPROM::read(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS("Reading from EEPROM ", address, dataLength);

    eeprom.readBlock(address, data, dataLength);
}

void ConfigurationStoreEEPROM::eraseAllAndReset() {
	DEBUG_PRINT("Clearing all EEPROM memory!");

	byte empty[TEMPBUFFSIZE] = {0};

	for (int i = 0; i < EEPROM_SIZE_IN_BYTES; i += TEMPBUFFSIZE) {
		DEBUG_PRINT_NUM("Erasing EEPROM block", i);
    	eeprom.writeBlock(i, empty, TEMPBUFFSIZE);
    }
}
