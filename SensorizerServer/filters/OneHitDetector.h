#ifndef OneHitDetector_h
#define OneHitDetector_h

//# include <string>
//using namespace std;

#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;

#include "OutputFilter.h"


class OneHitDetector : public OutputFilter {

	/**
	 * A debounce delay: The number of ticks to wait after a hit is triggered before sending another one.
	 *
	 * Any interstitial hits will be ignored.
	 * 
	 */
	public: int retriggerThreshold; 

	// unsigned because it needs to overflow without any major hiccups
	private: unsigned int ticksSinceLastTrigger;

	//for peak detector in setValue()
	private: double lastVal, triggeredVal;

	private: bool isRising;


	//private: OneHitListener triggerListener;
	
	
	public: void setValue(double value);

	OneHitDetector();

	private: void trigger();

	public: /*virtual*/ double value(); // dont use virtual as it's a little overboard for this.
	
	/** 
	 * Adds a listener for the trigger event. Note that this does not support multiple listeners!
	 * //@param l
	 */
	//public: void addListener(OneHitListener l);
};


#endif
