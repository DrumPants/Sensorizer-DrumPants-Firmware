#include <Encoder.h>

/**
 * Handles the knobs and buttons for the menu system.
 * 
 */

#ifndef Knobs_h_
#define Knobs_h_

#include <Arduino.h>
#include <Constants.h>
#include <SensorizerServer.h> 

#if IS_DUE 
  #define ENCODER_OPTIMIZE_INTERRUPTS
#else
  #define ENCODER_DO_NOT_USE_INTERRUPTS
#endif
#include <Encoder.h>

// set to 1 if using Encoder 1.1. lib, 0 if using 1.0
#define ENCODER_POSITION_MULTIPLIER 1

// set to a higher number than 1 to count more detents before switching a number
#define ENCODER_RESOLUTION 4


#if IS_DRUMPANTS

  #if PRESET >= PRESET_PREPRODUCTION

    #if PRESET >= PRESET_PREPRODUCTION_RED
      // new knob is backwards
      #define ENCODER_PIN_1 12
      #define ENCODER_PIN_2 11
    #else
      #define ENCODER_PIN_1 11
      #define ENCODER_PIN_2 12
    #endif

 
    #define ENCODER_MODE_SWITCH_PIN 32 

  #else 
    #define ENCODER_PIN_1 30
    #define ENCODER_PIN_2 11
    #define ENCODER_MODE_SWITCH_PIN 32 

  // for some reason our common pin is wired to a digital pin.
  // this should be wired to ground
    #define ENCODER_PIN_COMMON 12
  #endif 
#else
  #define ENCODER_PIN_1 8
  #define ENCODER_PIN_2 9
  #define ENCODER_MODE_SWITCH_PIN 10 
#endif

// these are indexes of the arrays holding scales. they should actually be enums or something.
typedef int ScaleId;
typedef int BankId;

#define ENABLE_LCD 1

#if ENABLE_LCD
  #include "LedInterface.h"
#endif



#if NOTE_PRESETS_ELEMENT_LENGTH == 6 
  #define NOTE_PRESETS_MELODIC_LENGTH 10
#else
  #define NOTE_PRESETS_MELODIC_LENGTH 66
#endif

#define NOTE_PRESETS_DRUMS_LENGTH 6

/*
//var n = [0, 60, 62, 63, 65, 67];
//major pent
//var n = [0, 60, 64, 65, 67, 69];
//minor pentatonic
var n = [0, 60, 63, 65, 67, 69];
var ns = [];
for (var i=0;i < 8; i++) { 
    for (j in n) {
        n[j] = n[j] + 1;
    }
    console.log(n);
}
*/




struct Knobs {

  SensorizerServer* server;

  Encoder* myEnc;

#if ENABLE_LCD
  LEDInterface lcd;
#endif

  int32_t position ;
  int32_t positionKey;
  long lastPos;

  bool lastButtonMode;

  void changeBank(int position);
  void changeScale(bool isMelodic, ScaleId scaleId);

public:
  Knobs();
  virtual void setup(SensorizerServer* server);
  virtual void check();
};


#endif
