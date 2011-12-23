#include "OneHitDetector.h"
#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;

#include "OutputFilter.h"


	
	//for peak detector in setValue()
	double OneHitDetector::lastVal = SensorizerServer.SENSOR_VALUE_NULL;

	bool OneHitDetector::isRising = false;

	OneHitListener OneHitDetector::triggerListener;
	
	
	/* (non-Javadoc)
	 * //@see OutputFilter#setValue(double)
	 */
	//@Override
	void OneHitDetector::setValue(double value) {
		//detect peak in signal and trigger one hit if so
		if (value < lastVal || value == SensorizerServer.SENSOR_VALUE_NULL) {
			if (isRising) {
				trigger();
				isRising = false;
			}
		}
		else {
			isRising = true;
		}
		
		lastVal = value;
	}

	
	void OneHitDetector::trigger() {
		if (triggerListener != null) {
			triggerListener.onTrigger();
		}
	}
	
	/* (non-Javadoc)
	 * //@see OutputFilter#value()
	 */
	//@Override
	public: virtual double value();
	
	/** 
	 * Adds a listener for the trigger event. Note that this does not support multiple listeners!
	 * //@param l
	 */
	void OneHitDetector::addListener(OneHitListener l) {
		triggerListener = l;
	}
