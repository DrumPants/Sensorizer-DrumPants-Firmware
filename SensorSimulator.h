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


	int getNextValue() {
		if (curIdx < 0) {
			return 0;
		}

		if (++curIdx < valuesSize) {
			return values[curIdx] * 1024;
		}
		else {
			curIdx = -1;
		}

		return 0;
	}

	void reset() {
		curIdx = 0;
	};
};


#endif