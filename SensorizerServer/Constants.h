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

#ifndef SensorizerConstants_h
#define SensorizerConstants_h

// set to 1 to enable a keyboard-based interface for simulating hits.
// connect to SerialUSB in with serial monitor to test: type keys '0'-'9' to "hit" various input pins.
#define ENABLE_TEST_INTERFACE 0


/******* hardware version type presets ********/

// for the beta DrumPants PCB:
// NOTE: all beta presets are >= 10 and it is used to set other constants!
#define PRESET_BETA 10
#define PRESET_BETA_RYAN 11
#define PRESET_BETA_STRETCHY 12
#define PRESET_BETA_NEIL_PEART 14

// for the beta pre-production Black PCB (April 2014)
#define PRESET_PREPRODUCTION 20

// for the beta pre-production Red PCB (May 2014)
#define PRESET_PREPRODUCTION_RED 21

// for the final pre-production Red PCB (Sept 2014)
#define PRESET_PREPRODUCTION_CHINA 22

// for the earlier prototypes
#define PRESET_NEON_GREEN_STRING 3
#define PRESET_VJACKET_LEATHER 2
#define PRESET_JEANS 1

#define PRESET PRESET_PREPRODUCTION_CHINA



// the firmware version is modifed by SKU flag which is bit 6
#define BOARD_SKU_MODIFIER_PRO (0x00)
#define BOARD_SKU_MODIFIER_BASIC (0x40)

// set to BOARD_SKU_MODIFIER_BASIC for Basic kit (no bluetooth),
// or BOARD_SKU_MODIFIER_PRO for Pro/Neil Peart kits (Bluetooth)
#define BOARD_SKU BOARD_SKU_MODIFIER_PRO

#define FIRMWARE_VERSION_NUM 31

// the firmware version is modifed by SKU flag which is bit 6
#define FIRMWARE_VERSION (BOARD_SKU | FIRMWARE_VERSION_NUM)

/******* END hardware version type presets ********/

// only enable jack switches on the new boards that have the proper pulldown resistors
#define ENABLE_JACK_SWITCHES (PRESET > PRESET_PREPRODUCTION_RED)


#define BLE_TYPE_NONE 0
#define BLE_TYPE_BLUEGIGA 1
#define BLE_TYPE_REDBEARLABS 2
#define BLE_TYPE_BROADCOM 3



#if PRESET >= PRESET_PREPRODUCTION

  #define BLE_TYPE BLE_TYPE_BROADCOM

  // for the Configurator
  #if PRESET == PRESET_PREPRODUCTION
    #define EEPROM_SIZE_IN_KBITS 1
  #else
    #define EEPROM_SIZE_IN_KBITS 4
  #endif

#elif PRESET == PRESET_VJACKET_LEATHER

  // to use BLE:
  //#define BLE_TYPE BLE_TYPE_BLUEGIGA
  //#define USE_HARDWARE_SERIAL 1

  // to use MIDI shield:
  #define BLE_TYPE BLE_TYPE_NONE
  #define USE_HARDWARE_SERIAL 0

#elif PRESET == PRESET_NEON_GREEN_STRING

  #define BLE_TYPE BLE_TYPE_REDBEARLABS
  #define USE_HARDWARE_SERIAL 1

#else
  #define BLE_TYPE BLE_TYPE_NONE
#endif

// disables firmata and only sends the MIDI notes (same as BLE)
#define ENABLE_SENDING_MIDI_OVER_USB 1

// can't use Firmata on USB and also debug print on USB:
#define ENABLE_FIRMATA (!ENABLE_DEBUG_PRINTING && !ENABLE_TEST_INTERFACE && !ENABLE_SENDING_MIDI_OVER_USB)

//uncomment this to set serial baud at bluetooth rate. otherwise, USB rate.
//# define IS_BLUETOOTH

#if PRESET >= PRESET_BETA
  #define IS_DUE 1
  #define IS_DRUMPANTS 1
#else
  #define IS_DUE 0 
  #define IS_DRUMPANTS 0
#endif

#define IS_BLE 0

#define ENABLE_TEST 0

// for the MIDI chip in MidiDevice class
#if BLE_TYPE == BLE_TYPE_BLUEGIGA
  #define BAUD_RATE_MIDI 19200
#elif BLE_TYPE == BLE_TYPE_REDBEARLABS
  #define BAUD_RATE_MIDI 57600
#else
  #define BAUD_RATE_MIDI 31250
#endif


#define BAUD_RATE_USB 115200
#define BAUD_RATE_BLUETOOTH 115200


#if BLE_TYPE == BLE_TYPE_BLUEGIGA
  #define BAUD_RATE_BLUETOOTH_LE 19200
#elif BLE_TYPE == BLE_TYPE_BROADCOM
  #define BAUD_RATE_BLUETOOTH_LE 115200

  // pre-production pants have the broadcom reset line attached!
  #define BLE_RESET_PIN 31

  // BLEIO1: connected to P0 of BLE
  #define BLE_CONFIG_PIN 8

  // BLEIO2: connected to P8 of BLE
  #define BLE_CONFIG_PIN2 7
#endif









/*** MIDI ******/

#ifdef MIDI_ENABLED

/**
 * If true, we'll send class-compliant MIDI packets over USB. (Requires special build of Arduino, hence the ifdef above. See README for details.)
 * If false, we'll only send non-compliant MIDI messages over the USB Serial, which will only work with the DrumPants 1.0 app.
 * 
 * Note, if true, it will still send the non-compliant messages over USB Serial AS WELL as the class-compliant MIDI messages. 
 * They show up as separate devices using the Composite USB device magic.
 */
#  define ENABLE_USB_MIDI 1
#else
#  define ENABLE_USB_MIDI 0
#endif

// the channel to send midi notes
#define MIDI_CHANNEL 0

// the channel to send midi CC messages of the sensor's index over BLE
// WARNING: if this is defined, the BLE latency will be much worse.
//#define MIDI_CHANNEL_SENSOR_IDX (MIDI_CHANNEL + 1)

#define ENABLE_LOOPER 0


// normal:
#define MIDI_NOTE_DURATION 1024



/*** END MIDI ******/







// the default num of ticks before a note retriggers
#define DEFAULT_RETRIGGER_THRESHOLD 30
#define DEFAULT_RETRIGGER_THRESHOLD_FOOT_PEDAL 170

// just disable this. it doesn't work very well. (DRUM-1051)
#define DEFAULT_SENSITIVITY_THRESHOLD 0.325

#if IS_DUE

  #define USE_HARDWARE_SERIAL 1


  // start off on the middle since our sensors only have 6 still.
  #if PRESET == PRESET_BETA_RYAN
    #define ANALOG_PIN_START 2
    #define ANALOG_PIN_END 8
  #elif PRESET == PRESET_BETA_STRETCHY
    #define ANALOG_PIN_START 2
    #define ANALOG_PIN_END 11
  #elif PRESET >= PRESET_PREPRODUCTION || PRESET == PRESET_BETA_NEIL_PEART
    #define ANALOG_PIN_START 0
    #define ANALOG_PIN_END 11
  #else
    #define ANALOG_PIN_START 1
    #define ANALOG_PIN_END 11
  #endif

  // TODO: these need to be updated for DUE (MORE NOTES!). hacking for now
  #define NOTE_PRESETS_ELEMENT_LENGTH 12


  #define ENABLE_MIDI_IN 1
#else

  // normal arduinos have 6 analog ins
  #define ANALOG_PIN_START 0
  #define ANALOG_PIN_END 5

  #define NOTE_PRESETS_ELEMENT_LENGTH 6

#endif

#if IS_BLE
  //#define BAUD_RATE BAUD_RATE_BLUETOOTH_LE
  #define BAUD_RATE BAUD_RATE_USB
  
  #define USE_HARDWARE_SERIAL 0
  
  // for testing:
  #define BAUD_RATE_MIDI BAUD_RATE_BLUETOOTH_LE
  
#elif IS_BLUETOOTH
  #define BAUD_RATE BAUD_RATE_BLUETOOTH
#else
  #define BAUD_RATE BAUD_RATE_USB
#endif


#endif