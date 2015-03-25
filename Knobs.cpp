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
  /**** generated with scale_generator.js ****/
  
  // KEY OF C
  // Scale major
  {36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55},
  {60, 62, 64, 65, 67, 69, 71, 72, 74, 76, 77, 79},
  {84, 86, 88, 89, 91, 93, 95, 96, 98, 100, 101, 103},


  // OTHER KEYS

  // Key 61
  // Scale major
  {37, 39, 41, 42, 44, 46, 48, 49, 51, 53, 54, 56},
  {61, 63, 65, 66, 68, 70, 72, 73, 75, 77, 78, 80},
  {85, 87, 89, 90, 92, 94, 96, 97, 99, 101, 102, 104},
  // Key 62
  // Scale major
  {38, 40, 42, 43, 45, 47, 49, 50, 52, 54, 55, 57},
  {62, 64, 66, 67, 69, 71, 73, 74, 76, 78, 79, 81},
  {86, 88, 90, 91, 93, 95, 97, 98, 100, 102, 103, 105},
  // Key 63
  // Scale major
  {39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58},
  {63, 65, 67, 68, 70, 72, 74, 75, 77, 79, 80, 82},
  {87, 89, 91, 92, 94, 96, 98, 99, 101, 103, 104, 106},
  // Key 64
  // Scale major
  {40, 42, 44, 45, 47, 49, 51, 52, 54, 56, 57, 59},
  {64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 81, 83},
  {88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105, 107},
  // Key 65
  // Scale major
  {41, 43, 45, 46, 48, 50, 52, 53, 55, 57, 58, 60},
  {65, 67, 69, 70, 72, 74, 76, 77, 79, 81, 82, 84},
  {89, 91, 93, 94, 96, 98, 100, 101, 103, 105, 106, 108},
  // Key 66
  // Scale major
  {42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 59, 61},
  {66, 68, 70, 71, 73, 75, 77, 78, 80, 82, 83, 85},
  {90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 107, 109},
  // Key 67
  // Scale major
  {43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 60, 62},
  {67, 69, 71, 72, 74, 76, 78, 79, 81, 83, 84, 86},
  {91, 93, 95, 96, 98, 100, 102, 103, 105, 107, 108, 110},
  // Key 68
  // Scale major
  {44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 61, 63},
  {68, 70, 72, 73, 75, 77, 79, 80, 82, 84, 85, 87},
  {92, 94, 96, 97, 99, 101, 103, 104, 106, 108, 109, 111},
  // Key 69
  // Scale major
  {45, 47, 49, 50, 52, 54, 56, 57, 59, 61, 62, 64},
  {69, 71, 73, 74, 76, 78, 80, 81, 83, 85, 86, 88},
  {93, 95, 97, 98, 100, 102, 104, 105, 107, 109, 110, 112},
  // Key 70
  // Scale major
  {46, 48, 50, 51, 53, 55, 57, 58, 60, 62, 63, 65},
  {70, 72, 74, 75, 77, 79, 81, 82, 84, 86, 87, 89},
  {94, 96, 98, 99, 101, 103, 105, 106, 108, 110, 111, 113},
  // Key 71
  // Scale major
  {47, 49, 51, 52, 54, 56, 58, 59, 61, 63, 64, 66},
  {71, 73, 75, 76, 78, 80, 82, 83, 85, 87, 88, 90},
  {95, 97, 99, 100, 102, 104, 106, 107, 109, 111, 112, 114},
  // Key 72
  // Scale major
  {48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67},
  {72, 74, 76, 77, 79, 81, 83, 84, 86, 88, 89, 91},
  {96, 98, 100, 101, 103, 105, 107, 108, 110, 112, 113, 115},





  // OTHER SCALES
  // Scale minor
  {36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55},
  {60, 62, 63, 65, 67, 68, 70, 72, 74, 75, 77, 79},
  {84, 86, 87, 89, 91, 92, 94, 96, 98, 99, 101, 103},
  // Scale melodicMinor
  {36, 38, 39, 41, 43, 45, 47, 48, 50, 51, 53, 55},
  {60, 62, 63, 65, 67, 69, 71, 72, 74, 75, 77, 79},
  {84, 86, 87, 89, 91, 93, 95, 96, 98, 99, 101, 103},
  // Scale gypsy
  {36, 38, 39, 41, 42, 44, 46, 48, 50, 51, 53, 54},
  {60, 62, 63, 65, 66, 68, 70, 72, 74, 75, 77, 78},
  {84, 86, 87, 89, 90, 92, 94, 96, 98, 99, 101, 102},
  // Scale persian
  {36, 37, 40, 41, 43, 44, 46, 48, 49, 52, 53, 55},
  {60, 61, 64, 65, 67, 68, 70, 72, 73, 76, 77, 79},
  {84, 85, 88, 89, 91, 92, 94, 96, 97, 100, 101, 103},

  // Alt Scale major
  {36, 38, 40, 47, 43, 45, 41, 48, 50, 52, 53, 55},
  {60, 62, 64, 71, 67, 69, 65, 72, 74, 76, 77, 79},
  {84, 86, 88, 95, 91, 93, 89, 96, 98, 100, 101, 103},
  // Alt Scale minor
  {36, 38, 39, 46, 43, 44, 41, 48, 50, 51, 53, 55},
  {60, 62, 63, 70, 67, 68, 65, 72, 74, 75, 77, 79},
  {84, 86, 87, 94, 91, 92, 89, 96, 98, 99, 101, 103},
  // Alt Scale melodicMinor
  {36, 38, 39, 47, 43, 45, 41, 48, 50, 51, 53, 55},
  {60, 62, 63, 71, 67, 69, 65, 72, 74, 75, 77, 79},
  {84, 86, 87, 95, 91, 93, 89, 96, 98, 99, 101, 103},
  // Alt Scale gypsy
  {36, 38, 39, 46, 42, 44, 41, 48, 50, 51, 53, 54},
  {60, 62, 63, 70, 66, 68, 65, 72, 74, 75, 77, 78},
  {84, 86, 87, 94, 90, 92, 89, 96, 98, 99, 101, 102},
  // Alt Scale persian
  {36, 37, 40, 46, 43, 44, 41, 48, 49, 52, 53, 55},
  {60, 61, 64, 70, 67, 68, 65, 72, 73, 76, 77, 79},
  {84, 85, 88, 94, 91, 92, 89, 96, 97, 100, 101, 103}



  // {58, 60, 63, 65, 67, 68, 70, 71, 73, 74, 74, 74},
  // {58, 60, 64, 65, 67, 69, 70, 72, 73, 75, 75, 75},
  // {58, 60, 65, 67, 68, 71, 72, 74, 75, 77, 77, 77},
  // {81, 82, 86, 87, 89, 91, 93, 94, 96, 97, 97, 97},
  // {63, 64, 68, 69, 71, 73, 74, 76, 77, 79, 79, 79},
  // {64, 65, 69, 70, 72, 74, 75, 77, 79, 80, 80, 80},
  // {92, 93, 97, 98, 100, 102, 103, 105, 106, 108, 108, 108},
  // {21, 22, 25, 27, 29, 31, 32, 34, 35, 37, 37, 37},
  // {40, 41, 44, 46, 48, 50, 51, 53, 54, 56, 56, 56},
  // {66, 66, 69, 71, 73, 75, 76, 78, 79, 81, 81, 81}
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
    Cowbell ,
    LowWoodBlock
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
    Cowbell,
    LowWoodBlock
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
    Cowbell,
    LowWoodBlock
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
    Cowbell,
    LowWoodBlock
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
    Maracas,
    LowWoodBlock
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
    Cowbell,
    LowWoodBlock
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

// changes drum scale offset
void Knobs::changeDrumScale(int positionKey) {

#define MIDI_GM2_DRUM_BANK_NUM_SOUNDS (MIDI_GM2_DRUM_BANK_END - MIDI_GM2_DRUM_BANK_START)    
    
  int scaleId;

  // create a new bank by offsetting all the notes in the current one 
  // cheap way of adding more banks.
  scaleId = abs(positionKey) % MIDI_GM2_DRUM_BANK_NUM_SOUNDS;

  byte offset = scaleId;
  byte newScale[NOTE_PRESETS_ELEMENT_LENGTH];

  for (int i = 0; i < NOTE_PRESETS_ELEMENT_LENGTH; i++) {
    // TODO: remember which drum bank we're on instead of always using the first one! (0 index)
    newScale[i] = (((NOTE_PRESETS_DRUMS[0][i] + offset) - MIDI_GM2_DRUM_BANK_START) % (MIDI_GM2_DRUM_BANK_NUM_SOUNDS )) + MIDI_GM2_DRUM_BANK_START;
  }

  server->loadNotes(newScale);

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

  int delta = (curPos - lastPos);
  
  lastButtonMode = buttonMode;
  lastPos = curPos;

  onKnobTurned(delta);

#if ENABLE_LCD
  lcd.check();
#endif
}


void Knobs::onKnobTurned(int delta) {

  if (lastButtonMode == HIGH) {
    // no button, change bank
    int newPos = position + delta;
    if (newPos != position) {
      DEBUG_PRINT_NUM("encoder: ", newPos);
      
      changeBank(newPos);

      position = newPos;
    }
  }
  else { //button is depressed. cheer up, button!
    // change scale
    
    int newPos = positionKey + delta;
    if (newPos != positionKey) {
      positionKey = newPos;
      DEBUG_PRINT_NUM("encoder key: ", positionKey);
      
      if (server->midiDevice->getBank() == 0x78) { //DRUMS
        changeDrumScale(positionKey);
      }
      else { //MELODIC
        changeScale(true, positionKey);
      }
    }
  }
}


bool Knobs::isKnobPressed() {
  return (this->lastButtonMode == LOW);
}
