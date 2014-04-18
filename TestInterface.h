#ifndef TEST_INTERFACE_H
#define TEST_INTERFACE_H

#if ENABLE_TEST_INTERFACE

#include "Debug.h"

#define TEST_HIT_MAX 1024

#define TEST_HIT_INCREMENT TEST_HIT_MAX / 4
#define TESTPINS_LENGTH 8

int testPins[TESTPINS_LENGTH] = {0,0,0,0,0,0,0,0};

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