#include "OneHitDetector.h"
#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;

#include "OutputFilter.h"


	//OneHitListener OneHitDetector::triggerListener;
		
	OneHitDetector::OneHitDetector() {
		this->lastVal = SensorizerServer::SENSOR_VALUE_NULL;
		this->isRising = false;
		this->triggeredVal = SensorizerServer::SENSOR_VALUE_NULL;

		this->retriggerThreshold = 5;
		this->ticksSinceLastTrigger = 0;
		this->sensitivityThreshold = DEFAULT_SENSITIVITY_THRESHOLD;



		highPassFilterConstant = 0;
		lowPassFilterConstant = 0;
		lastFilterOutputValue = 0;
		lastFilterInputValue = 0;
	}

    // need this otherwise it's virtual and xcode complains
    OneHitDetector::~OneHitDetector() {
        
    }
	
	
	/* (non-Javadoc)
	 * //@see OutputFilter#setValue(double)
	 */
	//@Override
	void OneHitDetector::setValue(double value) {
		ticksSinceLastTrigger++;
		triggeredVal = SensorizerServer::SENSOR_VALUE_NULL;


		// run pre filters

		//save cur raw val
		double _outputValue = value;

		if (lowPassFilterConstant > 0) {
			_outputValue = LOW_PASS_FILTER(lastFilterOutputValue, _outputValue, lowPassFilterConstant);
			//DEBUG_PRINT_NUMS("Low pass filter: ", value, _outputValue);
		}
		if (highPassFilterConstant > 0) {
			_outputValue = HIGH_PASS_FILTER(lastFilterOutputValue, lastFilterInputValue, _outputValue, highPassFilterConstant);
			// DEBUG_PRINT_NUMS("High pass lastFilterOutputValue, highPassFilterConstant ", lastFilterOutputValue, lastFilterInputValue);
			// DEBUG_PRINT_NUMS("High pass filter _outputValue, highPassFilterConstant: ", _outputValue, highPassFilterConstant);
			// DEBUG_PRINT_NUMS("High pass filter: ", value, _outputValue);
		}


		// detect peak in signal and trigger one hit if so
		// we detect peak using the filters, but we want to send out the non-filtered value if a hit is detected,
		// since the filtered peak is often much lower than the original hit.
		if (_outputValue < lastFilterOutputValue || value == SensorizerServer::SENSOR_VALUE_NULL) {
			if (isRising) {
				double delta = abs(lastFilterOutputValue - lowVal);
					
				if (sensitivityThreshold == 0 || delta > sensitivityThreshold) {
					trigger();
				}

				isRising = false;
			}
			else {
				lowVal = _outputValue;
			}
		}
		else {
			isRising = true;
		}
		

		lastFilterInputValue = value;
		lastFilterOutputValue = _outputValue;
		lastVal = value;
	}

	
	void OneHitDetector::trigger() {

		if (ticksSinceLastTrigger > retriggerThreshold) {
			ticksSinceLastTrigger = 0;

			DEBUG_PRINT_NUM("triggered NOTE: ", lastVal);

			// if (triggerListener != null) {
	// 			triggerListener.onTrigger();
	// 		}

			triggeredVal = lastVal;
		}
	}
	
	/* (non-Javadoc)
	 * //@see OutputFilter#value()
	 */
	//@Override
	//public: virtual double value();
	double OneHitDetector::value() {
		return triggeredVal; // return peak, not current val after peak
	}
	/** 
	 * Adds a listener for the trigger event. Note that this does not support multiple listeners!
	 * //@param l
	 */
	//void OneHitDetector::addListener(OneHitListener l) {
	//	triggerListener = l;
	//}
