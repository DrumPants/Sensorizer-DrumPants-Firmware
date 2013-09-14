#ifndef OneHitDetector_h
#define OneHitDetector_h

//# include <string>
//using namespace std;

#include "../SensorizerServer.h"
////import com.odbol.sensorizer.server.SensorizerServer;

#include "OutputFilter.h"


class OneHitDetector : OutputFilter {
	
	//for peak detector in setValue()
	private: double lastVal, triggeredVal;

	private: bool isRising;

	//private: OneHitListener triggerListener;
	
	
	/* (non-Javadoc)
	 * //@see OutputFilter#setValue(double)
	 */
	//@Override
	public: void setValue(double value);

	
	public: void trigger();

	OneHitDetector();
	
	/* (non-Javadoc)
	 * //@see OutputFilter#value()
	 */
	//@Override
	public: /*virtual*/ double value(); // dont use virtual as it's a little overboard for this.
	
	/** 
	 * Adds a listener for the trigger event. Note that this does not support multiple listeners!
	 * //@param l
	 */
	//public: void addListener(OneHitListener l);
};


#endif
