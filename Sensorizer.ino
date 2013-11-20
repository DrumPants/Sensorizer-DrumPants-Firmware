#include <Constants.h>  
#include <Debug.h>

#if !USE_HARDWARE_SERIAL
#include <SoftwareSerial.h>
#endif 

#include <SensorizerServer.h>    


SensorizerServer* server;


#define TOTAL_ANALOG_PINS 6

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

#define ENCODER_PIN_1 8
#define ENCODER_PIN_2 9
#define ENCODER_MODE_SWITCH_PIN 10



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
byte NOTE_PRESETS_MELODIC[NOTE_PRESETS_MELODIC_LENGTH][6] = {
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

byte NOTE_PRESETS_DRUMS[NOTE_PRESETS_DRUMS_LENGTH][6] = {
	{0, 57, 40, 48, 44, 41},
	{0, 49, 39, 60, 61, 43},
	{0, 52, 39, 67, 68, 43},
	{0, 53, 58, 76, 77, 54}
};



Encoder myEnc;

void setupKnobs() {
 myEnc.init(ENCODER_PIN_1, ENCODER_PIN_2);
  
 pinMode(ENCODER_MODE_SWITCH_PIN, INPUT);
 digitalWrite(ENCODER_MODE_SWITCH_PIN, HIGH); //enable pullup 

 // init other vars with whatever values are appropriate.
 // this allows us to change things immediately.
 checkKnobs();
}

int32_t position  = 32;
int32_t positionKey  = 36;

bool lastButtonMode = HIGH;

void checkKnobs() {
   //don't swap as soon as they release button: remember states for button pushed and not
  int buttonMode = digitalRead(ENCODER_MODE_SWITCH_PIN);
  if (lastButtonMode != buttonMode) {
     if (buttonMode == HIGH)
       myEnc.write(position * 4);
     else
       myEnc.write(positionKey * 4); 
  }
  
  
  int newPos = myEnc.read() / 4;
  
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
        server->loadNotes( NOTE_PRESETS_DRUMS[0] );
      }
      else {     
        DEBUG_PRINT_NUM("change bank: ", newInst); 
        
        server->midiDevice->setBank(MIDI_CHANNEL, 0x79, newInst); //MELODIC
        
        if (position > 127) {
           //we switched from drums, reload the note scales        
          server->loadNotes( NOTE_PRESETS_MELODIC[0] );
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
        int newInst = abs(positionKey % NOTE_PRESETS_DRUMS_LENGTH);
        server->loadNotes( NOTE_PRESETS_DRUMS[newInst] );
      }
      else { //MELODIC
        int newInst = abs(positionKey % NOTE_PRESETS_MELODIC_LENGTH);
        server->loadNotes( NOTE_PRESETS_MELODIC[newInst] );
        
      }
    }
  }
  
  lastButtonMode = buttonMode;
}


void setupServer() {
    server = new SensorizerServer();

    server->init();

    // load good drums preset to start
    server->loadNotes( NOTE_PRESETS_DRUMS[0] );

    // must wait a second for the MIDI device to boot up before it accepts our messages
    //delay(1000);

    server->midiDevice->setBank(MIDI_CHANNEL, 0x78); //DRUMS
}



/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/* timer variables */
unsigned long currentMillis;        // store the current value from millis()
unsigned long previousMillis;       // for comparison with currentMillis
int samplingInterval = 1;//19;          // how often to run the main loop (in ms)


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
  DEBUG_PRINT_SETUP

  setPinModeCallback(ENCODER_PIN_1, INPUT);
  setPinModeCallback(ENCODER_PIN_2, INPUT);
  setPinModeCallback(ENCODER_MODE_SWITCH_PIN, INPUT);

   
  DEBUG_PRINT("BEGIN SERIAL")
  //Serial.begin(BAUD_RATE);


  //SerialUSB.begin(BAUD_RATE);

  DEBUG_PRINT("SETUPSERVER")
  setupServer();

  DEBUG_PRINT("SETUPKNOBS")
  setupKnobs();

  DEBUG_PRINT("END SETUP")
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop() 
{
  byte pin, analogPin;

  Serial.println("loop ser");
  SerialUSB.println("loop usb");

  checkKnobs();
  
  /* SEND FTDI WRITE BUFFER - make sure that the FTDI buffer doesn't go over
   * 60 bytes. use a timer to sending an event character every 4 ms to
   * trigger the buffer to dump. */

  currentMillis = millis();
  if (currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;
    /* ANALOGREAD - do all analogReads() at the configured sampling interval */
    for(analogPin = 0; analogPin < TOTAL_ANALOG_PINS; analogPin++) {
          int val = analogRead(analogPin);
          
#if !ENABLE_TEST
          server->readPin(analogPin, val);
#endif          
 
    }
  }

#if ENABLE_LOOPER  
  // loop that shit! 
  server->tick();
#endif
  
#if ENABLE_TEST
  testUpdate();
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
    Serial.println("TEST: reset test"); 
  }
  
  
  int val = 0;
  if (testCounter > 0) {
    val = testCounter;
  }
  server->readPin(testPin, val);
}
#endif
