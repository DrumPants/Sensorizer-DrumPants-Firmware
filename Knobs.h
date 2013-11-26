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


#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h> 

#if IS_DRUMPANTS
  #define ENCODER_PIN_1 30
  #define ENCODER_PIN_2 11
  #define ENCODER_MODE_SWITCH_PIN 32 

  // for some reason our common pin is wired to a digital pin.
  // this should be wired to ground
  #define ENCODER_PIN_COMMON 12
#else
  #define ENCODER_PIN_1 8
  #define ENCODER_PIN_2 9
  #define ENCODER_MODE_SWITCH_PIN 10 
#endif

// these are indexes of the arrays holding scales. they should actually be enums or something.
typedef int ScaleId;
typedef int BankId;

#define ENABLE_LCD 0

#if ENABLE_LCD
  #include "LcdInterface.h"
#endif



#define NOTE_PRESETS_MELODIC_LENGTH 10
#define NOTE_PRESETS_DRUMS_LENGTH 4

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

  Encoder myEnc;

#if ENABLE_LCD
  LCDInterface lcd;
#endif

  int32_t position ;
  int32_t positionKey;

  bool lastButtonMode;

  void changeScale(bool isMelodic, ScaleId scaleId);

public:
  Knobs();
  void setup(SensorizerServer* server);
  void check();
};


#endif
