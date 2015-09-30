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

//
//    FILE: MultiSpeedI2CScanner.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.03
// PURPOSE: I2C scanner @different speeds
//    DATE: 2013-11-05
//     URL:
//
// Released to the public domain
//
#ifndef I2CScanner_H
#define I2CScanner_H

#include <Wire.h>
#include <Arduino.h>


#define SERIAL_TO_COMPUTER SerialToComputer


// scans devices from 50 to 800KHz I2C speeds.
// lower than 50 is not possible
// DS3231 RTC works on 800 KHz. TWBR = 2; (?)
long speed[] = { 
  50, 100, 200, 250, 400, 500, 800 };
const int speeds = sizeof(speed)/sizeof(speed[0]);

// DELAY BETWEEN TESTS
#define RESTORE_LATENCY  5    // for delay between tests of found devices.
bool delayFlag = false;

// MINIMIZE OUTPUT
bool printAll = true;
bool header = true;


uint32_t startScan;
uint32_t stopScan;

/**
 * Scans the I2C bus on all addresses.
 * @return the number of devices found.
 */
int I2Cscan()
{
  startScan = millis();
  uint8_t count = 0;

  if (header)
  {
    SERIAL_TO_COMPUTER.print(F("TIME\tDEC\tHEX\t"));
    for (uint8_t s = 0; s < speeds; s++)
    {
      SERIAL_TO_COMPUTER.print(F("\t"));
      SERIAL_TO_COMPUTER.print(speed[s]);
    }
    SERIAL_TO_COMPUTER.println(F("\t[KHz]"));
    for (uint8_t s = 0; s < speeds + 5; s++)
    {
      SERIAL_TO_COMPUTER.print(F("--------"));
    }
    SERIAL_TO_COMPUTER.println();
  }

  // TEST
  for (uint8_t address = 0; address < 128; address++)
  {
    bool printLine = printAll;
    uint8_t found[speeds];
    bool fnd = false;

    for (uint8_t s = 0; s < speeds ; s++)
    {
#ifdef TWBR      
      TWBR = (F_CPU/(speed[s]*1000) - 16)/2;
#else
    #warning "You are probably on Arduino Due - changing speeds will not work - only 100MHz"
#endif      
      Wire.beginTransmission (address);
      found[s] = Wire.endTransmission();
      fnd |= (found[s] == 0);
      // give device 5 millis
      if (fnd && delayFlag) delay(RESTORE_LATENCY);
    }

    if (fnd) count++;
    printLine |= fnd;

    if (printLine)
    {
      SERIAL_TO_COMPUTER.print(millis());
      SERIAL_TO_COMPUTER.print(F("\t"));
      SERIAL_TO_COMPUTER.print(address, DEC);
      SERIAL_TO_COMPUTER.print(F("\t0x"));
      SERIAL_TO_COMPUTER.print(address, HEX);
      SERIAL_TO_COMPUTER.print(F("\t"));

      for (uint8_t s = 0; s < speeds ; s++)
      {
        SERIAL_TO_COMPUTER.print(F("\t"));
        SERIAL_TO_COMPUTER.print(found[s]);//? F("V"):F("."));
      }
      SERIAL_TO_COMPUTER.println();
    }
  }

  stopScan = millis();
  if (header)
  {
    SERIAL_TO_COMPUTER.println();
    SERIAL_TO_COMPUTER.print(count);
    SERIAL_TO_COMPUTER.print(F(" devices found in "));
    SERIAL_TO_COMPUTER.print(stopScan - startScan);
    SERIAL_TO_COMPUTER.println(F(" milliseconds."));
  }

  return count;
}

#endif
