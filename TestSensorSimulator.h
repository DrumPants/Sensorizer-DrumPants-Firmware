#ifndef TEST_SENSOR_SIM_H
#define TEST_SENSOR_SIM_H

#include "Debug.h"
#include "SensorSimulator.h"
#include "SensorTestData.h"

#define TEST_HIT_MAX 1024

#define TEST_HIT_INCREMENT TEST_HIT_MAX / 4
#define TESTPINS_LENGTH 8


SensorSimulator testSensors[TESTPINS_LENGTH];


void test_initSensorSimulators() {
	int i = 0;
	// drumpads
	for (; i < 8; i++) {
		testSensors[i].setData(filtersTest_PiezoValues, sizeof(filtersTest_PiezoValues) * sizeof(double));
	}
	// continue with foot pedals
	for (; i < TESTPINS_LENGTH; i++) {
		testSensors[i].setData(filtersTest_Values, sizeof(filtersTest_Values) * sizeof(double));
	}
}



int testPins[TESTPINS_LENGTH] = {0,0,0,0,0,0,0,0};

void test_sensorSimTick() {
	
}

void test_hitSensor(int analogPin) {
  testPins[analogPin] = TEST_HIT_MAX;
}


int getCurrentTestVal(int analogPin) {
  if (analogPin >= TESTPINS_LENGTH) return 0;

  // make signal creep down if it got hit before
  if (testPins[analogPin] > 0) {
    testPins[analogPin] = max(0, testPins[analogPin] - TEST_HIT_INCREMENT);
  }

  return testPins[analogPin];
}

#endif