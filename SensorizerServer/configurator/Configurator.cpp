#include "Configurator.h"

bool Configurator::setField(SensorizerServer* server, byte sensorIdx, byte fieldIdx, byte value) {
	SensorOutput* output = server->sensorInputs[sensorIdx];
	
	if (output == NULL) {
		return false;
	}
	
	switch (fieldIdx) {


		case INRANGE_HIGH: {

				output->inRange.high = (float)(value) / 127.0f;

			}

			break;


		case INRANGE_LOW: {

				output->inRange.low = (float)(value) / 127.0f;

			}

			break;


		case OUTRANGE_HIGH: {

				output->outRange.high = (float)(value) / 127.0f;

			}

			break;


		case OUTRANGE_LOW: {

				output->outRange.low = (float)(value) / 127.0f;

			}

			break;


		case CUTOFFRANGE_HIGH: {

				output->cutoffRange.high = (float)(value) / 127.0f;

			}

			break;


		case CUTOFFRANGE_LOW: {

				output->cutoffRange.low = (float)(value) / 127.0f;

			}

			break;


		case MIDIMAPPING_MSGTYPE: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->msgType = value;
			


			}

			break;


		case MIDIMAPPING_CHANNEL: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->channel = value;
			


			}

			break;


		case MIDIMAPPING_NOTE: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->note = value;
			


			}

			break;


		case MIDIMAPPING_DURATION: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->duration = ((float)(value) / 127.0f) * 700;
			


			}

			break;


		case ONEHITDETECTOR_RETRIGGERTHRESHOLD: {

				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->retriggerThreshold = ((float)(value) / 127.0f) * 300;
			


			}

			break;


		case ONEHITDETECTOR_SENSITIVITYTHRESHOLD: {

				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return false;

				filter->sensitivityThreshold = (float)(value) / 127.0f;
			


			}

			break;



		default:
			return false;
	}

	return true;
}


byte Configurator::getField(SensorizerServer* server, byte sensorIdx, byte fieldIdx) {
	SensorOutput* output = server->sensorInputs[sensorIdx];
	
	if (output == NULL) {
		return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;
	}
	
	switch (fieldIdx) {


		case INRANGE_HIGH: {

				return output->inRange.high * 127;

			}

			break;


		case INRANGE_LOW: {

				return output->inRange.low * 127;

			}

			break;


		case OUTRANGE_HIGH: {

				return output->outRange.high * 127;

			}

			break;


		case OUTRANGE_LOW: {

				return output->outRange.low * 127;

			}

			break;


		case CUTOFFRANGE_HIGH: {

				return output->cutoffRange.high * 127;

			}

			break;


		case CUTOFFRANGE_LOW: {

				return output->cutoffRange.low * 127;

			}

			break;


		case MIDIMAPPING_MSGTYPE: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;

				return filter->msgType;
			


			}

			break;


		case MIDIMAPPING_CHANNEL: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;

				return filter->channel;
			


			}

			break;


		case MIDIMAPPING_NOTE: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;

				return filter->note;
			


			}

			break;


		case MIDIMAPPING_DURATION: {

				MidiMapping* filter = (MidiMapping*)output->getMidiMapping(0); // TODO DERRRP???

				if (filter == NULL)
					return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;

				return ((float)(filter->duration) / 700.0f) * 127;
			


			}

			break;


		case ONEHITDETECTOR_RETRIGGERTHRESHOLD: {

				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;

				return ((float)(filter->retriggerThreshold) / 300.0f) * 127;
			


			}

			break;


		case ONEHITDETECTOR_SENSITIVITYTHRESHOLD: {

				OneHitDetector* filter = (OneHitDetector*)output->getOutputFilter(0); // TODO DERRRP???

				if (filter == NULL)
					return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;

				return filter->sensitivityThreshold * 127;
			


			}

			break;



		default:
			return CONFIGURATOR_ERROR_RETURN_CODE_FAILURE;
	}
}
