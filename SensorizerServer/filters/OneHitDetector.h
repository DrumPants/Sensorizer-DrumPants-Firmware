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

	/***
	 * The amount of change of a spike from base to peak before it's considered a hit.
	 */
	double sensitivityThreshold;
	

	/**
	 * Set to > 0 to enable the low or high pass filters.
	 * They are applied before any other processing, but after inversion.
	 * 
	 */
	float lowPassFilterConstant,
		  highPassFilterConstant;

	double 	lastFilterOutputValue,
		 	lastFilterInputValue;

	// unsigned because it needs to overflow without any major hiccups
	private: unsigned int ticksSinceLastTrigger;

	//for peak detector in setValue()
	private: double lastVal, triggeredVal, lowVal;

	private: bool isRising;


	//private: OneHitListener triggerListener;
	
	
	public: void setValue(double value);
    
    OneHitDetector();
    ~OneHitDetector();

	private: void trigger();

	public: /*virtual*/ double value(); // dont use virtual as it's a little overboard for this.
	
	/** 
	 * Adds a listener for the trigger event. Note that this does not support multiple listeners!
	 * //@param l
	 */
	//public: void addListener(OneHitListener l);
};


#endif
