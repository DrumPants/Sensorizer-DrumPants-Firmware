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

#ifndef SensorizerDebug_h
#define SensorizerDebug_h

#define ENABLE_DEBUG_PRINTING 0

// if 1, we'll relay all debug messages from the BLE module's HCI UART to the SerialToComputer
#define ENABLE_DEBUG_PRINTING_RELAY_FROM_BLE 0

#if IS_DUE
	// SerialUSB is BROKEN
	#define SerialToComputer SerialUSB
#else
	#define SerialToComputer Serial
#endif

// don't use this function! it fucks things up for some reason!
#define DEBUG_PRINT_SETUP SerialToComputer.begin(57600);SerialToComputer.println(F("Debug printing STARTED"));

#define PRINT(text) SerialToComputer.println(F(text));SerialToComputer.println(F("-----"));
#define PRINT_NUM(text, num) SerialToComputer.print(F(text));SerialToComputer.println(num, DEC);SerialToComputer.println(F("-----"));
#define PRINT_NUMS(text, num, num2) SerialToComputer.print(F(text));SerialToComputer.print(num, DEC);SerialToComputer.print(F(", "));SerialToComputer.println(num2, DEC);SerialToComputer.println(F("-----"));
 
#if ENABLE_DEBUG_PRINTING
	#define DEBUG_PRINT(text) PRINT(text)
	#define DEBUG_PRINT_NUM(text, num) PRINT_NUM(text, num)
	#define DEBUG_PRINT_NUMS(text, num, num2) PRINT_NUMS(text, num, num2)
#else
	#define DEBUG_PRINT(text)   
	#define DEBUG_PRINT_NUM(text, num)   
	#define DEBUG_PRINT_NUMS(text, num, num2)
#endif

#endif