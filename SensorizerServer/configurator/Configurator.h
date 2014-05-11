/***
	Configurator. Converts JSON to MIDI CC notes to structs.
***/

#ifndef Configurator_h
#define Configurator_h

#include "SensorizerServer.h"

#include "filters/OneHitDetector.h"
#include "filters/NullableOutputFilter.h"

/**** MIDI CC MESSAGE CONSTANTS FOR FIELDS ****/

#define	INRANGE_HIGH 1  
#define	INRANGE_LOW 2  
#define	OUTRANGE_HIGH 3  
#define	OUTRANGE_LOW 4  
#define	CUTOFFRANGE_HIGH 5  
#define	CUTOFFRANGE_LOW 6  
#define	MIDIMAPPING_MSGTYPE 7  
#define	MIDIMAPPING_CHANNEL 8  
#define	MIDIMAPPING_NOTE 9  
#define	MIDIMAPPING_DURATION 10  
#define	ONEHITDETECTOR_RETRIGGERTHRESHOLD 11  
#define	ONEHITDETECTOR_SENSITIVITYTHRESHOLD 12  

#define CONFIGURATOR_FIELDS_START INRANGE_HIGH
#define CONFIGURATOR_FIELDS_END ONEHITDETECTOR_SENSITIVITYTHRESHOLD


#define CONFIGURATOR_ERROR_RETURN_CODE_FAILURE 0

class Configurator {


public:
	/***
		Sets the indexed field in output to value.

	***/
	static bool setField(SensorizerServer* server, byte sensorIdx, byte fieldIdx, byte byteVal);

	/***
		Gets the current value for the given field index.

		Returns CONFIGURATOR_ERROR_RETURN_CODE_FAILURE if sensor or field is not found.

	***/
	static byte getField(SensorizerServer* server, byte sensorIdx, byte fieldIdx);

};


#endif