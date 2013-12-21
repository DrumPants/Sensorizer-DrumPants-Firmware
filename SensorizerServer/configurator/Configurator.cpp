#include "Configurator.h"


bool Configurator::setField(SensorOutput* output, byte fieldIdx, float value) {

	switch (fieldIdx) {
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
		default:
			return false;
	}

	return true;
}