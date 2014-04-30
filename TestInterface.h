#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

#if ENABLE_TEST_INTERFACE

#include "Debug.h"

#include "I2CScanner.h"
#include "TestBLE.h"

#define TEST_HIT_MAX 1024

#define TEST_HIT_INCREMENT TEST_HIT_MAX / 4
#define TESTPINS_LENGTH 8

int testPins[TESTPINS_LENGTH] = {0,0,0,0,0,0,0,0};


void testInterfaceSetup() {

  SerialToComputer.println("========================");
  SerialToComputer.println("= SENSORIZER TEST MENU =");
  SerialToComputer.println("========================");
  SerialToComputer.println("Available key commands:");
  SerialToComputer.println("");
  SerialToComputer.println("0-9 : test drumpad hit");
  SerialToComputer.println("e   : scan for EEPROM");
  SerialToComputer.println("b   : test BLE programming");
  SerialToComputer.println("u   : test BLE PUART communication");
  SerialToComputer.println("r   : run all tests");
  SerialToComputer.println("========================");

}

/**
 * [scanForEEPROM description]
 * @return 0 on success, 1 on failure.
 */
int scanForEEPROM() {

  SerialToComputer.println("========================");
  SerialToComputer.println("= SCANNING FOR EEPROM  =");
  SerialToComputer.println("========================");

  int numFound = I2Cscan();

  return (numFound == 0) ? 1 : 0;
}

/**
 * [testBLEProgramming description]
 * @return 0 on success, 1 on failure.
 */
int testBLEProgramming() {
  SerialToComputer.println("========================");
  SerialToComputer.println("= TESTING BLE PROG     =");
  SerialToComputer.println("========================");

  int numFailedAttempts =  ble_testBLEProgramming();

  if (numFailedAttempts == 0) {
    SerialToComputer.println("SUCCESS! BLE responded for programming.");
  }
  else {
    SerialToComputer.println("FAILED! BLE did not respond for programming.");
  }

  return (numFailedAttempts > 0) ? 1 : 0;
}

/**
 * [testBLEPUART description]
 * @return 0 on success, 1 on failure.
 */
int testBLEPUART() {
  SerialToComputer.println("========================");
  SerialToComputer.println("= TESTING BLE PUART    =");
  SerialToComputer.println("========================");


  return 0;
}



void testInterfaceUpdate() {

  // for (int i = 0; i < TESTPINS_LENGTH; i++) {
  //   if (testPins[i] > 0) {
  //     testPins[i] = max(0, testPins[i] - TEST_HIT_INCREMENT);
  //   }
  // }

  // check if the user has triggered a new hit
  while (SerialToComputer.available() > 0) {
    byte cmd = SerialToComputer.read();
    byte cmdPin = cmd - '0'; // they type the number of the pin to activate

    if (cmdPin >= 0 && cmdPin < TESTPINS_LENGTH) {
      testPins[cmdPin] = TEST_HIT_MAX;
    }
    else {
      switch (cmd) {
        case 'e':
          scanForEEPROM();
          break;
        case 'b':
          testBLEProgramming();
          break;
        case 'u':
          testBLEPUART();
          break;
        case 'r':

          int numTestsFailed = scanForEEPROM() +
              testBLEProgramming() +
              testBLEPUART();

          // also play a little sumthin' to indicate doneness.
          testPins[1] = TEST_HIT_MAX;

          if (numTestsFailed == 0) {

            SerialToComputer.println("========================");
            SerialToComputer.println("= All tests PASSED     =");
            SerialToComputer.println("========================");
          }
          else {
            SerialToComputer.println("========================");
            SerialToComputer.print("= ");
            SerialToComputer.print(numTestsFailed);
            SerialToComputer.print("/3 tests FAILED        =");
            SerialToComputer.println("========================");
          }
          break;
      }
    }
  }
}

int getCurrentTestVal(byte analogPin) {
  if (analogPin >= TESTPINS_LENGTH) return 0;

  // make signal creep down if it got hit before
  if (testPins[analogPin] > 0) {
    testPins[analogPin] = max(0, testPins[analogPin] - TEST_HIT_INCREMENT);
  }

  return testPins[analogPin];
}

#endif

#endif