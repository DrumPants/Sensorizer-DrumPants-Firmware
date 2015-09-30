/**
 * Copyright 2011-2015 DrumPants, Inc.
 * 
 * http://developers.drumpants.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OutputFilter_h
#define OutputFilter_h


#define LOW_PASS_FILTER(lastOut, curIn, k) (lastOut + k * (curIn - lastOut))
#define HIGH_PASS_FILTER(lastOut, lastIn, curIn, k) ( k * (lastOut + curIn - lastIn) )


class OutputFilter {
	/**
	 * this is the class name of the specific filter, so that GSON can make sure to instantiate the correct filter
	 * from a generic array. 
	 * 
	 * Make sure to override this with your class's unique name and add to //@see com.odbol.sensorizer.gson.OutputFilterInstanceCreator
	 * 
	 */
	////@Expose
	//public: const static char* type;
	
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
