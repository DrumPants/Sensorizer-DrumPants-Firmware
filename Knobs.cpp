#include "Knobs.h"
#include "general_midi_sounds.h"

#define PRESETS_END_MELODIC 127
#define PRESETS_START_DRUMS (PRESETS_END_MELODIC + 1)
// this is so each drum scale is actually also a bank. TODO make this better
#define PRESETS_END_DRUMS (PRESETS_START_DRUMS + NOTE_PRESETS_DRUMS_LENGTH)

byte NOTE_PRESETS_MELODIC[NOTE_PRESETS_MELODIC_LENGTH][NOTE_PRESETS_ELEMENT_LENGTH] = {


#if NOTE_PRESETS_ELEMENT_LENGTH == 6  
        {58, 60, 63, 65, 67, 68},
        {58, 60, 64, 65, 67, 69},
        {58, 60, 65, 67, 68, 71},
        {22, 82, 86, 87, 89, 91},
        {63, 64, 68, 69, 71, 73},
        {64, 65, 69, 70, 72, 74},
        {33, 93, 97, 98, 100, 102},
	{21, 22, 25, 27, 29, 31},
	{40, 41, 44, 46, 48, 50},
	{65, 66, 69, 71, 73, 75}/*,
	{8, 68, 71, 73, 75, 77}*/
#else
  {58, 60, 63, 65, 67, 68, 70, 71, 73, 74, 74},
  {58, 60, 64, 65, 67, 69, 70, 72, 73, 75, 75},
  {58, 60, 65, 67, 68, 71, 72, 74, 75, 77, 77},
  {81, 82, 86, 87, 89, 91, 93, 94, 96, 97, 97},
  {63, 64, 68, 69, 71, 73, 74, 76, 77, 79, 79},
  {64, 65, 69, 70, 72, 74, 75, 77, 79, 80, 80},
  {92, 93, 97, 98, 100, 102, 103, 105, 106, 108, 108},
  {21, 22, 25, 27, 29, 31, 32, 34, 35, 37, 37},
  {40, 41, 44, 46, 48, 50, 51, 53, 54, 56, 56},
  {66, 66, 69, 71, 73, 75, 76, 78, 79, 81, 81}
#endif
};

byte NOTE_PRESETS_DRUMS[NOTE_PRESETS_DRUMS_LENGTH][NOTE_PRESETS_ELEMENT_LENGTH] = {

#if NOTE_PRESETS_ELEMENT_LENGTH == 6
{
    OpenHi_hat,
    CrashCymbal2,
    ElectricSnare,
    HighMidTom, 
    PedalHi_hat, 
    LowFloorTom
  },
  {
    OpenHi_hat,
    CrashCymbal1,
    HandClap,
    HighBongo, 
    LowBongo, 
    HighFloorTom},
  {
    OpenHi_hat,
    ChineseCymbal,
    HandClap,
    HighAgogo, 
    LowAgogo, 
    HighFloorTom},
  {
    OpenHi_hat,
    RideBell,
    Vibra_slap,
    HiWoodBlock, 
    LowWoodBlock, 
    Tambourine
  },
  {
    OpenHi_hat,
    ChineseCymbal,
    Tambourine,
    HighTimbale, 
    LowTimbale, 
    HighFloorTom
  },
  {
    OpenHi_hat,
    ChineseCymbal,
    ElectricSnare,
    MuteHiConga, 
    OpenHiConga, 
    LowConga
  }
#else
  {
    AcousticSnare,  // A2
    Low_MidTom,     // B2
    HandClap,       // C2
    HighMidTom,     // D2
    CrashCymbal2,   // A1
    OpenHi_hat,     // B1
    SplashCymbal,   // C1
    HighFloorTom,   // D1
    AcousticBassDrum,// A3
    PedalHi_hat,    // B3
    Cowbell 
  },
  {
    AcousticSnare,    // A2
    LowBongo,         // B2
    HandClap,         // C2
    Low_MidTom,       // D2
    CrashCymbal1,     // A1
    HighBongo,        // B1
    Claves,           // C1
    SplashCymbal,     // D1
    BassDrum1,        // A3
    PedalHi_hat,      // B3
    Cowbell
  },
  {
    ElectricSnare,    // A2
    ChineseCymbal,    // B2
    HandClap,         // C2
    HighAgogo,        // D2
    LowAgogo,         // A1
    HighFloorTom,     // B1
    SplashCymbal,     // C1
    Claves,           // D1
    BassDrum1,        // A3
    PedalHi_hat,      // B3
    Cowbell
  },
  {
    AcousticSnare,
    CrashCymbal2,
    Claves,
    HiWoodBlock, 
    LowWoodBlock, 
    Tambourine, 
    HighAgogo, 
    LowAgogo, 
    BassDrum1,
    PedalHi_hat,
    Cowbell
  },
  {
    AcousticSnare,
    LowTimbale,
    Tambourine,
    HighFloorTom, 
    ChineseCymbal, 
    HighTimbale,
    Cowbell, 
    Castanets, 
    BassDrum1,
    PedalHi_hat,
    Maracas
  },
  {
    AcousticSnare,
    ChineseCymbal,
    ElectricSnare,
    MuteHiConga, 
    OpenHiConga, 
    LowConga,
    Cowbell, 
    CrashCymbal2, 
    BassDrum1,
    PedalHi_hat,
    Cowbell
  }

#endif  
};

Knobs::Knobs() { 
  lastPos = 0; 
  position  = PRESETS_END_MELODIC + 1; // start with DRUMS!
  positionKey  = 1;
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
 //check();
 
 changeBank(position);
 //changeScale(false, positionKey);
}

void Knobs::changeBank(int newPos) {
  // give drums 12 spots so we don't miss them with the bad encoder.
  int newInst = abs(newPos) % PRESETS_END_DRUMS; 
  
  //if (newInst < 0)
  //  newInst = 128 - newInst;
  if (newInst > PRESETS_END_MELODIC) {
    DEBUG_PRINT_NUM("change drum bank: ", newInst);
    server->midiDevice->setBank(MIDI_CHANNEL, 0x78); //DRUMS
    
    //previous was melodic, load drum preset
    changeScale(false, (newInst - PRESETS_START_DRUMS) % NOTE_PRESETS_DRUMS_LENGTH);

#if ENABLE_LCD
    lcd.changeBank("Drums", newInst);
#endif        
  }
  else {     
    DEBUG_PRINT_NUM("change sound bank: ", newInst); 
    
    server->midiDevice->setBank(MIDI_CHANNEL, 0x79, newInst); //MELODIC

    if (position > PRESETS_END_MELODIC) {
       //we switched from drums, reload the note scales        
      changeScale(true, 0);
    }
    
    // print to LCD after so we trample changeScale()'s printing.    
#if ENABLE_LCD        
    lcd.changeBank("Smurds!", newInst);
#endif
  }
}


void Knobs::changeScale(bool isMelodic, int positionKey) {
  int scaleId;

  if (isMelodic) {
    scaleId = abs(positionKey) % NOTE_PRESETS_MELODIC_LENGTH;
    server->loadNotes( NOTE_PRESETS_MELODIC[scaleId] );
  }
  else {
    scaleId = abs(positionKey) % NOTE_PRESETS_DRUMS_LENGTH;
    server->loadNotes( NOTE_PRESETS_DRUMS[scaleId] );
  }

#if ENABLE_LCD
  lcd.changeScale(scaleId);
#endif
}

void Knobs::check() {
   //don't swap as soon as they release button: remember states for button pushed and not
  int buttonMode = digitalRead(ENCODER_MODE_SWITCH_PIN);
  // if (lastButtonMode != buttonMode) {
  //    if (buttonMode == HIGH)
  //      myEnc->write(position * ENCODER_POSITION_MULTIPLIER);
  //    else
  //      myEnc->write(positionKey * ENCODER_POSITION_MULTIPLIER); 
  // }
  
  
  int curPos = (myEnc->read() / ENCODER_POSITION_MULTIPLIER) / ENCODER_RESOLUTION;

  if (buttonMode == HIGH) {
    // no button, change bank
    int newPos = position + (curPos - lastPos);
    if (newPos != position) {
      DEBUG_PRINT_NUM("encoder: ", newPos);
      
      changeBank(newPos);

      position = newPos;
    }
  }
  else { //button is depressed. cheer up, button!
    // change scale
    
    int newPos = positionKey + (curPos - lastPos);
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
  lastPos = curPos;

#if ENABLE_LCD
  lcd.check();
#endif
}
