#ifndef NullableOutputFilter_h
#define NullableOutputFilter_h

#include "OutputFilter.h"


class NullableOutputFilter : public OutputFilter {

	double val;
	double lastVal;

public:
	/**
	 * the default value to send if behavior == SEND_DEFAULT_ON_FIRST
	 */
	double defaultValue;
	

	NullableOutputFilter(double defaultValue = 0);

	// WTF these are already declared in superclass. why do i need to declare them again???
	void setValue(double value);
	double value();	
};

#endif