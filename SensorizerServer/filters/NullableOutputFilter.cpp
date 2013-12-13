#include "NullableOutputFilter.h"


#include "../SensorizerServer.h"


NullableOutputFilter::NullableOutputFilter(double defaultValue) {
		this->defaultValue = defaultValue;
		lastVal = val = SensorizerServer::SENSOR_VALUE_NULL;
}

void NullableOutputFilter::setValue(double value) {
	if (value == SensorizerServer::SENSOR_VALUE_NULL) {

		// SEND_DEFAULT_ON_FIRST:
		if (val == defaultValue || lastVal == SensorizerServer::SENSOR_VALUE_NULL) {
			value = SensorizerServer::SENSOR_VALUE_NULL;
		}
		else {
			value = defaultValue;
		}
		val = SensorizerServer::SENSOR_VALUE_NULL; //make sure the lastVal is set to null and not the default!
	}

	lastVal = val;
	val = value;
}

double NullableOutputFilter::value() {
	return val;
}