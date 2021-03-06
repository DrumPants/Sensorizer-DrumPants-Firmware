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

/**
 * Functions for controlling, reseting, and programming the BLE device.
 */

#ifndef BLE_DEVICE_H
#define BLE_DEVICE_H

#include <Arduino.h>

#include <Debug.h>
#include <Constants.h>

#define BLE_PROGRAMMING_TX_PIN 1 

void ble_reset() {

#ifdef BLE_RESET_PIN

  // broadcom has reset line attached, make sure it's not reseting!
  // active-low.

  // reset the BLE
  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_RESET_PIN, LOW); 
  delay(2); // 20uS to reset

  digitalWrite(BLE_RESET_PIN, HIGH); 

  delay(7); // ~5ms to warm up and boot

#else
  #warning "BLE_RESET_PIN not defined. Your BLEDevice functions will do nothing."
#endif

}

/**
 * Resets for programming or for normal operation.
 * @param shouldPrepareForProgramming if true, the BLE chip will wait for programming commands. if false, it will start running.
 */
void ble_resetAndPrepare(bool shouldPrepareForProgramming) {

  // if it starts up with the TX line high.
  // TODO: Not sure what setting the TX pin will do for the actual communication later. Will the UART class successfully regain control of the TX line?
  pinMode(BLE_PROGRAMMING_TX_PIN, OUTPUT);
  digitalWrite(BLE_PROGRAMMING_TX_PIN, shouldPrepareForProgramming ? HIGH : LOW); 

  ble_reset();
}

void ble_setup() {

#ifdef BLE_RESET_PIN

  // first, set programming HCI UART to low, because the BLE will just wait to be programmed 
  ble_resetAndPrepare(false);

  // TODO: uncomment this to implement reprograming the BLE firmware via its HCI UART
  Serial.begin(115200);

#endif

}



#endif