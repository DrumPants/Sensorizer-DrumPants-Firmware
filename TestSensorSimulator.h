#ifndef TEST_SENSOR_SIM_H
#define TEST_SENSOR_SIM_H

#include "Debug.h"
#include "SensorSimulator.h"
#include "SensorTestData.h"
#include "Metro.h"

#define TEST_HIT_MAX 1024

#define TEST_HIT_INCREMENT TEST_HIT_MAX / 4
#define TESTPINS_LENGTH 8


SensorSimulator testSensors[TESTPINS_LENGTH];

Metro test_snareMetro;
bool test_isRhythmOn = false;

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


void test_hitSensor(int analogPin) {
  testSensors[analogPin].start();
}


int getCurrentTestVal(int analogPin) {
  if (analogPin >= TESTPINS_LENGTH) return 0;

  return testSensors[analogPin].getNextValue() * 1024;
}



void test_sensorSimToggleRhythm(int tempo) {
	test_isRhythmOn = !test_isRhythmOn;

	if (test_isRhythmOn) {
		test_snareMetro.timePerTick = tempo;
	}
}

void test_sensorSimTick() {
	
	if (test_isRhythmOn) {
		
		if (test_snareMetro.hasTicked()) {
			test_hitSensor(2);
		}
	}
}


#endif