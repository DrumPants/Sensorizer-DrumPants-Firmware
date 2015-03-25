/**
 * Handles the knobs and buttons for the menu system.
 * 
 */

#ifndef KnobsAndButtons_h_
#define KnobsAndButtons_h_

#include <Arduino.h>
#include <Constants.h>
#include <SensorizerServer.h> 
#include "Knobs.h"

 #include <Metro.h>




struct KnobsAndButtons : public Knobs {

  bool downButtonPressed,
        upButtonPressed,
        isTurningKnob;
  
  long debounceTimer;

  byte volume;

  void setVolume(int vol);

public:
  KnobsAndButtons();


  void check();
  void onKnobTurned(int delta);
};


#endif
