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
		case 20: {
				MidiMapping* m = (MidiMapping*)output->getMidiMapping(0); // assume we only do one midi mapping

				if (m == NULL)
					return false;

				m->note = (value * 127);
			}
			break;
		default:
			return false;
	}

	return true;
}