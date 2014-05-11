#include "ConfigurationStoreEEPROM.h"

#include "Debug.h"
#include <SensorizerServer.h>

ConfigurationStoreEEPROM::ConfigurationStoreEEPROM(SensorizerServer* s)  : ConfigurationStore(s) {
	
}

void ConfigurationStoreEEPROM::write(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS("Writing to EEPROM ", address, dataLength);
}

void ConfigurationStoreEEPROM::read(unsigned int address, byte* data, int dataLength) {
	DEBUG_PRINT_NUMS("Reading from EEPROM ", address, dataLength);
}
