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