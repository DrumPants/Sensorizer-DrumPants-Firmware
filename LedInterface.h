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

#include <SevenSegmentController.h>

/**
 * Handles the knobs and buttons for the menu system.
 * 
 */

#ifndef LEDInterface_h_
#define LEDInterface_h_

#include <Arduino.h>
#include <Constants.h>
#include <SensorizerServer.h>

#include <SevenSegmentController.h>


// these are indexes of the arrays holding scales. they should actually be enums or something.
typedef int ScaleId;
typedef int BankId;
 
struct LEDInterface {

	// holds the current bank for temporary printing to something else.
	BankId curBank;

	int numTicksLeftForTemporary;

	SevenSegmentController* led;

public: 

	LEDInterface() {
		led = new SevenSegmentController();
	};

	~LEDInterface() {
		delete led;
	};

	void setup();
	void check();

	void changeBank(const char* label, BankId bankId);
	void changeScale(ScaleId scaleId);


	void showTemporarily(int num);
};



 #endif
