#ifndef OutputFilter_h
#define OutputFilter_h

#include <string>
using namespace std;

class OutputFilter {
	/**
	 * this is the class name of the specific filter, so that GSON can make sure to instantiate the correct filter
	 * from a generic array. 
	 * 
	 * Make sure to override this with your class's unique name and add to //@see com.odbol.sensorizer.gson.OutputFilterInstanceCreator
	 * 
	 */
	////@Expose
	public: virtual const static string type;
	
	/**
	 * set the input value to process
	 * 
	 * //@param value 
	 * 	NOTE: this can accept SensorizerSystem.SENSOR_VALUE_NULL, which means the sensor value is null and should not be sent.
	 */
	public: virtual void setValue(double value);

	/**
	 * returns the filtered value
	   
	 * //@return
	 * NOTE: this can return SensorizerSystem.SENSOR_VALUE_NULL, which means the sensor value is null and should not be sent.
	 */
	public: virtual double value();
};


#endif
