#include "ConfigurationStoreEEPROM.h"

#include "Debug.h"
#include <SensorizerServer.h>

ConfigurationStoreEEPROM::ConfigurationStoreEEPROM() {
	ConfigurationStore::ConfigurationStore();

}

void ConfigurationStoreEEPROM::write(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS(F("Writing to EEPROM "), address, dataLength);
}

void ConfigurationStoreEEPROM::read(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS(F("Reading from EEPROM "), address, dataLength);
}