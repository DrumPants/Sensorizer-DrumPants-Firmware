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

  // current bpm of metronome.
  int bpm;

  byte volume;

  void setVolume(int vol);

public:
  KnobsAndButtons();


  void check();
  void onKnobTurned(int delta);
};


#endif
