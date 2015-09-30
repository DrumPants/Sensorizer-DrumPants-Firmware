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

#ifndef NullableOutputFilter_h
#define NullableOutputFilter_h

#include "OutputFilter.h"


class NullableOutputFilter : public OutputFilter {

	double val;
	double lastVal;

public:
	/**
	 * the default value to send if behavior == SEND_DEFAULT_ON_FIRST
	 */
	double defaultValue;
	

	NullableOutputFilter(double defaultValue = 0);

	// WTF these are already declared in superclass. why do i need to declare them again???
	void setValue(double value);
	double value();	
};

#endif