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

#ifndef TranslatingMidiRepeater_h
#define TranslatingMidiRepeater_h

#include <Arduino.h>
#include "Constants.h"
#include "Debug.h"

#include "MidiRepeater.h"

class SensorizerServer;

/***

	Repeats all Midi messages sent from a MidiDevice to the BLE chip,
	translating the notes into static sensor indexes using the current scale.

***/
class TranslatingMidiRepeater : public MidiRepeater {

	SensorizerServer* server;

public: 

	TranslatingMidiRepeater(SensorizerServer* server);

	/***
	The listener function. Called by a sensor input whenever it sends an output.

	***/
	void onSendOutput(byte cmd, byte data1, byte data2);

};


#endif