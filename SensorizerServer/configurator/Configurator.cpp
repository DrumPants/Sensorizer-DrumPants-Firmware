#include "Configurator.h"

/**** MIDI CC MESSAGE CONSTANTS FOR FIELDS ****/

#define	INRANGE_HIGH 1  
#define	INRANGE_LOW 2  
#define	OUTRANGE_HIGH 3  
#define	OUTRANGE_LOW 4  
#define	CUTOFFRANGE_HIGH 5  
#define	CUTOFFRANGE_LOW 6  
#define	MULTIPLYVAL 7  
#define	ADDVAL 8  
#define	ISINVERT 9  
#define	MIDIMAPPING_MSGTYPE 10  
#define	MIDIMAPPING_CHANNEL 11  
#define	MIDIMAPPING_NOTE 12  
#define	MIDIMAPPING_DURATION 13  
#define	MIDIMAPPING_MIDIDEVICENAME 14  
#define	ONEHITDETECTOR_RETRIGGERTHRESHOLD 15  
#define	ONEHITDETECTOR_SENSITIVITYTHRESHOLD 16  


bool Configurator::setField(SensorOutput* output, byte fieldIdx, float value) {

	switch (fieldIdx) {


		case INRANGE_HIGH: {

				output->inRange.high = value;

			}

			break;


		case INRANGE_LOW: {

				output->inRange.low = value;

			}

			break;


		case OUTRANGE_HIGH: {

				output->outRange.high = value;

			}

			break;


		case OUTRANGE_LOW: {

				output->outRange.low = value;

			}

			break;


		case CUTOFFRANGE_HIGH: {

				output->cutoffRange.high = value;

			}

			break;


		case CUTOFFRANGE_LOW: {

				output->cutoffRange.low = value;

			}

			break;


		case MULTIPLYVAL: {

				output->multiplyVal = value;

			}

			break;


		case ADDVAL: {

				output->addVal = value;

			}

			break;


		case ISINVERT: {

				output->isInvert = value;

			}

			break;

/*
		case MIDIMAPPING_MSGTYPE: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->msgType = (value);
			


			}

			break;
*/

		case MIDIMAPPING_CHANNEL: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->channel = (value * 127);
			


			}

			break;


		case MIDIMAPPING_NOTE: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->note = (value * 127);
			


			}

			break;


		case MIDIMAPPING_DURATION: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->duration = (value * 700);
			


			}

			break;


		case ONEHITDETECTOR_RETRIGGERTHRESHOLD: {

				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->retriggerThreshold = (value);
			


			}

			break;


		case ONEHITDETECTOR_SENSITIVITYTHRESHOLD: {

				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->sensitivityThreshold = (value);
			


			}

			break;



/*
		case 1:
			output->inRange.high = value;

			break;
		case 3:
			output->cutoffRange.low = value;

			break;
		case 10: {
				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->retriggerThreshold = value;
			}
			break;		
		case 20: {
				MidiMapping* m = (MidiMapping*)output->getMidiMapping(0); // assume we only do one midi mapping

				if (m == NULL)
					return false;

				m->note = (value * 127);
			}
			break;
*/
		default:
			return false;
	}

	return true;
}