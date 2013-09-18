#include "OneHitDetector.h"
#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;

#include "OutputFilter.h"



	//OneHitListener OneHitDetector::triggerListener;
	
	
	OneHitDetector::OneHitDetector() {
		this->lastVal = SensorizerServer::SENSOR_VALUE_NULL;
		this->isRising = false;
		this->triggeredVal = SensorizerServer::SENSOR_VALUE_NULL;
	}
	
	
	/* (non-Javadoc)
	 * //@see OutputFilter#setValue(double)
	 */
	//@Override
	void OneHitDetector::setValue(double value) {
		triggeredVal = SensorizerServer::SENSOR_VALUE_NULL;

		//detect peak in signal and trigger one hit if so
		if (value < lastVal || value == SensorizerServer::SENSOR_VALUE_NULL) {
			if (isRising) {
				trigger();

		DEBUG_PRINT_NUM("triggered NOTE: ", lastVal);
				isRising = false;
			}
		}
		else {
			isRising = true;
		}
		
		lastVal = value;
	}

	
	inline void OneHitDetector::trigger() {
		// if (triggerListener != null) {
// 			triggerListener.onTrigger();
// 		}

		triggeredVal = lastVal;
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
