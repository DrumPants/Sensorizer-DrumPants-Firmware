/***
	Configurator. Converts JSON to MIDI CC notes to structs.
***/

#ifndef Configurator_h
#define Configurator_h

#include "SensorizerServer.h"

#include "filters/OneHitDetector.h"
#include "filters/NullableOutputFilter.h"



class Configurator {


public:
	/***
		Sets the indexed field in output to value.

	***/
	static bool setField(SensorOutput* output, byte fieldIdx, float value);

};


#endif