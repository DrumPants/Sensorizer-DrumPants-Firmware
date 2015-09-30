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

#ifndef SensorSimulator_H
#define SensorSimulator_H

#include "Debug.h"

class SensorSimulator {

	int curIdx;
	const double* values;

	int valuesSize;

public:

	SensorSimulator() {
		valuesSize = 0;
		curIdx = -1;
	}


	// size = sizeof(values) * sizeof(double)
	void setData(const double values[], int size) {
		this->values = values;
		valuesSize = size;
	}

	double getNextValue() {
		if (curIdx < 0) {
			return 0;
		}

		if (++curIdx < valuesSize) {
			return values[curIdx];
		}
		else {
			curIdx = -1;
		}

		return 0;
	}

	void start() {
		curIdx = 0;
	};
};


#endif