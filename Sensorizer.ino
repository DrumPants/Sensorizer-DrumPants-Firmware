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
 * This is the main file. It loads up the SensorizerServer and all the UI classes, 
 * and coordinates them all. It reads the sensors in a loop and feeds them into
 * the server which sends them on.
 */

#include <Constants.h>  
#include <Debug.h>

#if !USE_HARDWARE_SERIAL
#include <SoftwareSerial.h>
#endif 

#include <SevenSegmentController.h>

#include <SensorizerServer.h>  

#include "SensorJackSwitches.h"

#include <Heartbeat.h>

#if ENABLE_FIRMATA
  #include <Firmata.h>

  // how many ticks it takes before sending a Firmata update. 
  // this needs to be throttled because otherwise we overload the Serial buffer and it freezes.
  #define FIRMATA_UPDATE_RATE_THROTTLE 10

// how many ticks it's been since the last Firmata update. 
int firmataThrottleCount = 0;  
#endif


#if ENABLE_MIDI_IN
  #include <utility/MidiInput.h>

  
  #include "Debug.h"
  #include <SensorizerServer.h>

// need to include this here so ConfigurationStoreEEPROM can use it.
  #include <Wire.h>
  #include <I2C_eeprom.h>

  #include "ConfigurationStoreEEPROM.h"

MidiInput* midiIn;

ConfigurationStore* configStore;
#endif


#if IS_DUE 
  #define ENCODER_OPTIMIZE_INTERRUPTS
#else
  #define ENCODER_DO_NOT_USE_INTERRUPTS
#endif
#include <Encoder.h>

#if PRESET >= PRESET_PREPRODUCTION
  #include "KnobsAndButtons.h"
#else
  #include "Knobs.h"
#endif

// for intro sound
#include "IntroSounds.h"

// need to include this here so BLEDevice can use it.
#include "BLEDevice.h"


/**
 *
 * THE MEAT OF THE WHOLE OPERATION.
 */
SensorizerServer* server;

/**
 * Handles the UI: knobs and buttons and LEDs.
 */
Knobs* knobs;

Heartbeat heartbeat;

#if ENABLE_TEST_INTERFACE
  #include "TestInterface.h"
#endif

#define ENABLE_TIMESTAMP_TEST 0
#if ENABLE_TIMESTAMP_TEST

#define TEST_TIMESTAMPS_LENGTH 512
int testTimestampsIdx = 0;
unsigned long testTimestamps[TEST_TIMESTAMPS_LENGTH];

#endif



/**
 * Set to 1 to send one sensor's data with timestamps as a CSV to the USB.
 * Used for the sensorizerTester Xcode app.
 */
#define ENABLE_TIMESTAMP_VAL_TEST 0
#if ENABLE_TIMESTAMP_VAL_TEST

#define TIMSTAMP_VAL_TEST_SENSOR_IDX_TO_LOG 0
#define TEST_VAL_TIMESTAMPS_LENGTH 256

int testValTimestampsIdx = 0;
unsigned long testValTimestamps[TEST_VAL_TIMESTAMPS_LENGTH];
int testVals[TEST_VAL_TIMESTAMPS_LENGTH];

#endif


void setupServer() {
    // must wait a bit for the MIDI device to boot up before it accepts our messages
    delay(500);
    
    
    server = new SensorizerServer();
    server->init();

    
    // the setup will load good drums preset to start
#if PRESET >= PRESET_PREPRODUCTION
    knobs = new KnobsAndButtons();
#else
    knobs = new Knobs();    
#endif
    knobs->setup(server);

#if ENABLE_MIDI_IN
    configStore = new ConfigurationStoreEEPROM(server);

    // listen for MIDI messages from BLE to change config
    midiIn = new MidiInput(server, configStore);

    // now load all saved sensor config data, clearing defaults.
    configStore->loadSensors();
#endif    
}



/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/* timer variables */
unsigned long currentMillis;        // store the current value from millis()
unsigned long previousMillis;       // for comparison with currentMillis
int samplingInterval = 1;//19;          // how often to run the main loop (in ms). set to 0 to disable samplingThrottle.


// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(byte pin, int mode)
{

  switch(mode) {
  // case ANALOG:
  //   //if (IS_PIN_ANALOG(pin)) {
  //   //  if (IS_PIN_DIGITAL(pin)) {
  //       pinMode(pin, INPUT); // disable output driver
  //       digitalWrite(pin, LOW); // disable internal pull-ups
  //   //  }
  //  // }
  //   break;
  case INPUT:
    //if (IS_PIN_DIGITAL(pin)) {
      pinMode(pin, INPUT); // disable output driver
      digitalWrite(pin, LOW); // disable internal pull-ups
    //}
    break;
  case OUTPUT:
    //if (IS_PIN_DIGITAL(pin)) {
      digitalWrite(pin, LOW); // disable PWM
      pinMode(pin, OUTPUT);
    //}
    break;
  }
}













/*==============================================================================
 * SETUP()
 *============================================================================*/
void setup() 
{
  // set up the debug printer
  //DEBUG_PRINT_SETUP

#if IS_DUE


  ble_setup(); 

  // this is needed for debug printing on native USB port,
  // as well as Firmata and MIDIRepeater
  SerialUSB.begin(BAUD_RATE_USB);

  #if ENABLE_FIRMATA
  Firmata.setFirmwareVersion(0, 1);
  Firmata.begin(SerialUSB);
  #endif
#else
  Serial.begin(BAUD_RATE);
#endif


  DEBUG_PRINT("SETUPSERVER")
  setupServer();

  DEBUG_PRINT("PLAYING INTRO SOUNDS")
  
  playIntro(server);

//server->startMetronome(90, 4);

#if ENABLE_TEST_INTERFACE
  delay(5000); // wait for them to connect the serial monitor
  testInterfaceSetup();
#endif

  initSensorJackSwitches();

  DEBUG_PRINT("END SETUP")
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop() 
{
  byte analogPin;

 // Serial.println("loop ser");
 // SerialUSB.println("loop usb");

  //checkKnobs();
  knobs->check();
  
  heartbeat.tick();

#if ENABLE_FIRMATA
  // while(Firmata.available()) {
  //   Firmata.processInput();
  // }
#endif  

#if ENABLE_MIDI_IN
  midiIn->check();
#endif


#if ENABLE_TIMESTAMP_TEST
  unsigned long microTimestamp = micros();

#endif

  ////////////////////////////
  // check sensors!
  ////////////////////////////

  currentMillis = millis();
  if (samplingInterval == 0 || currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;

    /* ANALOGREAD - do all analogReads() at the configured sampling interval */
    for(analogPin = ANALOG_PIN_START; analogPin <= ANALOG_PIN_END; analogPin++) {
        byte pinIdx = analogPin - ANALOG_PIN_START;
        int val;


        // so the problem is that isSensorPluggedIn() only detects whether the foot pedal is plugged in, not the rest of the DrumPads.
        // and the problem with that is if they press all the way down on the foot pedal, it will register as disconnected.
        // therefore we should not use this method as of yet.
        if (!ENABLE_JACK_SWITCHES || isSensorPluggedIn(analogPin)) {
          val = analogRead(analogPin);
        }
        else {
          // default to 0 if not plugged in? is that a good idea for foot pedals?
          val = 0;
        }
         

#if ENABLE_TEST_INTERFACE
        val = getCurrentTestVal(analogPin);
#endif


#if ENABLE_TIMESTAMP_VAL_TEST

        if (pinIdx == TIMSTAMP_VAL_TEST_SENSOR_IDX_TO_LOG) {

          testValTimestamps[testValTimestampsIdx] = micros();
          testVals[testValTimestampsIdx] = val;

          if (++testValTimestampsIdx >= TEST_VAL_TIMESTAMPS_LENGTH) {
            testValTimestampsIdx = 0;

            //SerialUSB.println("ValTimestamps:");
            unsigned long firstValTimestamp = testValTimestamps[0];
            for (int i = 0; i < TEST_VAL_TIMESTAMPS_LENGTH; i++) {
              SerialUSB.print(testValTimestamps[i]);
              SerialUSB.print(',');
              SerialUSB.println(testVals[i]);
            }
          }
        }

        val = 0; // disable all notes so we don't pollute the CSV with garbage
#endif  

        ////////////////////////////
        // process and send notes!
        ////////////////////////////
        
        //DEBUG_PRINT_NUM("check pin ", pinIdx);  
#if !ENABLE_TEST
        server->readPin(pinIdx, val);
#endif          

#if ENABLE_FIRMATA
      if (firmataThrottleCount == FIRMATA_UPDATE_RATE_THROTTLE) {
        // TODO: send average!
        Firmata.sendAnalog(analogPin, val); 
      }
#endif 
    }

#if ENABLE_FIRMATA
    if (++firmataThrottleCount > FIRMATA_UPDATE_RATE_THROTTLE) {
        firmataThrottleCount = 0;
    }
#endif
  }


#if ENABLE_TIMESTAMP_TEST
  unsigned long microTimestampDelta = micros() - microTimestamp;
  

  if (SerialUSB.getWriteError() > 0) {
      SerialUSB.println("ERROR WRITING TO USB");
      SerialUSB.clearWriteError();
  }


  testTimestamps[testTimestampsIdx] = microTimestampDelta;

  if (++testTimestampsIdx >= TEST_TIMESTAMPS_LENGTH) {
    testTimestampsIdx = 0;

    SerialUSB.println("Timestamps:");
    unsigned long firstTimestamp = testTimestamps[0];
    for (int i = 0; i < TEST_TIMESTAMPS_LENGTH; i++) {
      SerialUSB.println(testTimestamps[i]);
    }
  }
#endif  

  // relay all debug messages from BLE 
#if ENABLE_DEBUG_PRINTING && ENABLE_DEBUG_PRINTING_RELAY_FROM_BLE && defined(BLE_RESET_PIN)

  while(Serial.available()) {
    SerialUSB.write(Serial.read());
  }

  #if ENABLE_ECHO_PUART_FROM_BLE
  while(Serial1.available()) {
    SerialUSB.write(Serial1.read());
  }
  #endif  
#endif

  // trigger looper and/or metronome events
  server->tick();
  
// for class-compliant USB MIDI support. flush everything that was sent this cycle.  
#if ENABLE_USB_MIDI
  MidiUSB.flush();
#endif


#if ENABLE_TEST
  testUpdate();
#endif


#if ENABLE_TEST_INTERFACE
  testInterfaceUpdate();
  return;
#endif
}



#if ENABLE_TEST
#define TEST_MAX 90
#define TEST_INTERVAL 50000

int testCounter = -TEST_INTERVAL;
int testPin = 1;

void testUpdate() {
  if (++testCounter > TEST_MAX) {
    testCounter = -TEST_INTERVAL;
    
    // try next note once we've done a full one here.
    // testPin = (testPin + 1) % 4;
    DEBUG_PRINT("TEST: reset test");
  }
  
  
  int val = 0;
  if (testCounter > 0) {
    val = testCounter;
  }
  server->readPin(testPin, val);
}
#endif



