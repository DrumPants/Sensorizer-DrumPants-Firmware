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