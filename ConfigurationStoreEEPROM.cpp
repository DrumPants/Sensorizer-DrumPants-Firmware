#include "ConfigurationStoreEEPROM.h"

#include "Debug.h"
#include <SensorizerServer.h>


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
