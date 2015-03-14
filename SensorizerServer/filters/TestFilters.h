#ifndef TEST_FILTERS_H
#define TEST_FILTERS_H

#include "OutputFilter.h"

#ifndef DEBUG_PRINT_NUMS
#include <stdio.h>

#define DEBUG_PRINT_NUMS(msg, num1, num2) printf("%s %f, %f\n", msg, num1, num2)

#endif

#include "SensorTestData.h"

/**
 * Unit test for the lowpass/highpass filters.
 * 
 * @return [description]
 */
bool filtersTest_checkPreFilters() {
	double _outputValue = 0;
	double _inputValue = 0;
	float lowPassFilterConstant = 0;//0.8;
	float highPassFilterConstant = 0.8;
	
	for (int i = 1; i < sizeof(filtersTest_Values) / sizeof(filtersTest_Values[0]); i++) {
		double val = filtersTest_Values[i];
	
		double lastOutputValue = _outputValue;
		double lastInputValue = _inputValue;

		//save cur raw val
		_inputValue = val;
		_outputValue = val;

		if (lowPassFilterConstant > 0) {
			_outputValue = LOW_PASS_FILTER(lastOutputValue, _outputValue, lowPassFilterConstant);
			DEBUG_PRINT_NUMS("Low pass filter: ", val, _outputValue);
		}
		if (highPassFilterConstant > 0) {
			_outputValue = HIGH_PASS_FILTER(lastOutputValue, lastInputValue, _outputValue, highPassFilterConstant);
			DEBUG_PRINT_NUMS("High pass filter: ", val, _outputValue);
		}
	}


	return false;
}


#endif