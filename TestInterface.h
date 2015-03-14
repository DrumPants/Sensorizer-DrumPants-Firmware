#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

#if ENABLE_TEST_INTERFACE

#include "Debug.h"

#include "I2CScanner.h"
#include "TestBLE.h"
#include <utility/TestConfigurator.h>
#include <utility/TestFilters.h>

#include "TestSensorSimulator.h"

void testInterfaceSetup() {

  while (!SerialToComputer) {
    
  }

  SerialToComputer.println("========================");
  SerialToComputer.println("= SENSORIZER TEST MENU =");
  SerialToComputer.println("========================");
  SerialToComputer.println("Available key commands:");
  SerialToComputer.println("");
  SerialToComputer.println("0-9 : test drumpad hit");
  SerialToComputer.println("j   : test disconnection detector");
  SerialToComputer.println("e   : scan for EEPROM");
  SerialToComputer.println("c   : test configurator");
  SerialToComputer.println("D   : wipe all configurator EEPROM memory! (capital D)");
  SerialToComputer.println("a   : test config reporting");
  SerialToComputer.println("b   : test BLE programming");
  SerialToComputer.println("u   : test BLE PUART communication");
  SerialToComputer.println("n   : run all unit tests");
  SerialToComputer.println("r   : run all tests");
  SerialToComputer.println("========================");


  test_initSensorSimulators();
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
 * [testConfigurator description]
 * @return 0 on success, 1 on failure.
 */
int testConfigurator() {

  SerialToComputer.println("========================");
  SerialToComputer.println("=  TEST CONFIGURATOR   =");
  SerialToComputer.println("========================");

  int numErrors = configurator_testEEPROM(midiIn, server, configStore);

  numErrors += configurator_testGetVersion(midiIn);

  if (numErrors == 0) {
    SerialToComputer.println("SUCCESS! Configurator saved correctly.");
  }
  else {
    SerialToComputer.print("FAILED! Configurator failed with ");
    SerialToComputer.print(numErrors);
    SerialToComputer.println(" errors.");
  }

  return numErrors;
}

int testResetEEPROM() {

  SerialToComputer.println("===============================");
  SerialToComputer.println("=  RESET TO FACTORY DEFAULTS  =");
  SerialToComputer.println("===============================");

  configStore->eraseAllAndReset();

  SerialToComputer.println(" Erased all memory!");

  return 0;
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

/**
 * [testBLEPUART description]
 * @return 0 on success, 1 on failure.
 */
int testConfigReport() {
  SerialToComputer.println("========================");
  SerialToComputer.println("= TESTING CONFIG REPORT=");
  SerialToComputer.println("========================");

  configurator_testSendConfig(midiIn);

  return 0;
}

/**
 * [testSensorJackSwitches tests if jack switches are working.]
 * @return 0 if at least one jack (3 sensors) is plugged in, 1 on failure.
 */
int testSensorJackSwitches() {
  int numSensorsPluggedIn = 0;


  SerialToComputer.println("========================");
  SerialToComputer.println("= TESTING SENSOR JACKS =");
  SerialToComputer.println("========================");

  for (int i = ANALOG_PIN_START; i < ANALOG_PIN_END; i++) {
 
    SerialToComputer.print("Sensor ");
    SerialToComputer.print(i);

    if (isSensorPluggedIn(i)) {
      SerialToComputer.println(" plugged in.");
      numSensorsPluggedIn++;
    }
    else {
      SerialToComputer.println(" unplugged!");
    }
  }

  // if one jack is plugged in, that means at least 3 sensors should have been counted.
  return (numSensorsPluggedIn > 2) ? 0 : 1;
}


int testUnitTests() {
  return filtersTest_checkPreFilters() ? 0 : 1;
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
      test_hitSensor(cmdPin);
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
        case 'c':
          testConfigurator();
          break;
        case 'D':
          testResetEEPROM();
          break;
        case 'a':
          testConfigReport();
          break;
        case 'j':
          testSensorJackSwitches();
          break;
        case 'n':
          testUnitTests();
          break;
        case 'r':

          int numTestsFailed = scanForEEPROM() +
              testBLEProgramming() +
              testBLEPUART() +
              testConfigurator() +
              // and finally reset to factory defaults so it doesn't load rando stuff.
              testResetEEPROM();

          // also play a little sumthin' to indicate doneness.
          testPins[1] = TEST_HIT_MAX;

          if (numTestsFailed == 0) {

            SerialToComputer.println("========================");
            SerialToComputer.println("= All tests PASSED     =");
            SerialToComputer.println("========================");
          }
          else {
            SerialToComputer.println("========================");
            SerialToComputer.print("=   ");
            SerialToComputer.print(numTestsFailed);
            SerialToComputer.print("/4 tests FAILED      =");
            SerialToComputer.println("========================");
          }
          break;
      }
    }
  }

  test_sensorSimTick();
}


#endif

#endif