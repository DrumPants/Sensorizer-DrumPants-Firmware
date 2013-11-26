#include "Knobs.h"

byte NOTE_PRESETS_MELODIC[NOTE_PRESETS_MELODIC_LENGTH][NOTE_PRESETS_ELEMENT_LENGTH] = {
        {0, 60, 63, 65, 67, 68},
        {0, 60, 64, 65, 67, 69},
        {0, 60, 65, 67, 68, 71},
        {22, 82, 86, 87, 89, 91},
        {4, 64, 68, 69, 71, 73},
        {5, 65, 69, 70, 72, 74},
        {33, 93, 97, 98, 100, 102},
	{0, 22, 25, 27, 29, 31},
	{1, 41, 44, 46, 48, 50},
	{6, 66, 69, 71, 73, 75}/*,
	{8, 68, 71, 73, 75, 77}*/
};

byte NOTE_PRESETS_DRUMS[NOTE_PRESETS_DRUMS_LENGTH][NOTE_PRESETS_ELEMENT_LENGTH] = {
	{0, 57, 40, 48, 44, 41},
	{0, 49, 39, 60, 61, 43},
	{0, 52, 39, 67, 68, 43},
	{0, 53, 58, 76, 77, 54}
};

Knobs::Knobs() {  
  position  = 32;
  positionKey  = 36;
  lastButtonMode = HIGH;
}

void Knobs::setup(SensorizerServer* server) {
 this->server = server;

 myEnc = new Encoder(ENCODER_PIN_1, ENCODER_PIN_2);
  
 pinMode(ENCODER_MODE_SWITCH_PIN, INPUT);
 digitalWrite(ENCODER_MODE_SWITCH_PIN, HIGH); //enable pullup 

#ifdef ENCODER_PIN_COMMON
 // on the drumpants beta, the common pin is connected to digital pin instead of ground. fake it.
 pinMode(ENCODER_PIN_COMMON, OUTPUT);
 digitalWrite(ENCODER_PIN_COMMON, LOW); 
#endif


#if ENABLE_LCD
  lcd.setup();
#endif

 // init other vars with whatever values are appropriate.
 // this allows us to change things immediately.
 check();
}


void Knobs::changeScale(bool isMelodic, int positionKey) {

  if (isMelodic) {
    ScaleId scaleId = abs(positionKey % NOTE_PRESETS_MELODIC_LENGTH);
    server->loadNotes( NOTE_PRESETS_MELODIC[scaleId] );
  }
  else {
    ScaleId scaleId = abs(positionKey % NOTE_PRESETS_DRUMS_LENGTH);
    server->loadNotes( NOTE_PRESETS_DRUMS[scaleId] );
  }

#if ENABLE_LCD
  lcd.changeScale(positionKey);
#endif
}

void Knobs::check() {
   //don't swap as soon as they release button: remember states for button pushed and not
  int buttonMode = digitalRead(ENCODER_MODE_SWITCH_PIN);
  if (lastButtonMode != buttonMode) {
     if (buttonMode == HIGH)
       myEnc->write(position * 4);
     else
       myEnc->write(positionKey * 4); 
  }
  
  
  int newPos = myEnc->read() / 4;
  
  if (buttonMode == HIGH) {
    // no button, change bank
    if (newPos != position) {
      DEBUG_PRINT_NUM("encoder: ", newPos);
      int newInst = newPos % 129;
      if (newInst < 0)
        newInst = 128 - newInst;
      if (newInst > 127) {
        DEBUG_PRINT_NUM("change bank: ", newInst);
        server->midiDevice->setBank(MIDI_CHANNEL, 0x78); //DRUMS
        
        //previous was melodic, load drum preset
        changeScale(false, 0);

#if ENABLE_LCD
        lcd.changeBank("Drums", newInst);
#endif        
      }
      else {     
        DEBUG_PRINT_NUM("change bank: ", newInst); 
        
        server->midiDevice->setBank(MIDI_CHANNEL, 0x79, newInst); //MELODIC

#if ENABLE_LCD        
        lcd.changeBank("Smurds!", newInst);
#endif
        if (position > 127) {
           //we switched from drums, reload the note scales        
          changeScale(true, 0);
        }
      }
      
      position = newPos;
    }
  }
  else { //button is depressed. cheer up, button!
    // change scale
    if (newPos != positionKey) {
      positionKey = newPos;
      DEBUG_PRINT_NUM("encoder key: ", positionKey);
      
      if (server->midiDevice->getBank() == 0x78) { //DRUMS
        changeScale(false, positionKey);
      }
      else { //MELODIC
        changeScale(true, positionKey);
      }
    }
  }
  
  lastButtonMode = buttonMode;

#if ENABLE_LCD
  lcd.check();
#endif
}