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

#define IS_DRUMPANTS 1
#define IS_DUE 1

#include <Arduino.h>
#include <SevenSegmentController.h>

#define TEST_PRINT 1
#define TEST_SET 0
#define TEST TEST_PRINT

SevenSegmentController led;

void setup() {
  led.setup();
   
   SerialUSB.begin(57600);
}

long interval = 1000;
long curCount = 0;

long long lastUpdate = 0;

const int updateInterval = 20;


void loop() {
  

  long long curMillis = millis();
  if (curMillis - lastUpdate > updateInterval) {
    ++curCount;
    int digit = ((double)curCount / (double)interval) * 10.0;
    
    if (curCount > interval) {
      curCount = 0;
     SerialUSB.println(curCount); 
     SerialUSB.println(digit); 
    }
    
     SerialUSB.println(digit); 
  //SerialUSB.println(digit);
#if TEST == TEST_PRINT    
    led.print(curCount / 10);
#elif TEST == TEST_SET
    led.setDigit(0, '4');
    led.setDigit(1, '2');
#endif    
    
    lastUpdate = curMillis;
  }

  led.tick();
}
