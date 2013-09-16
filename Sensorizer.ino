#include <Constants.h>  

#if !USE_HARDWARE_SERIAL
#include <SoftwareSerial.h>
#endif 

#include <SensorizerServer.h>    


SensorizerServer server;


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
    if (newPos != position) {
      DEBUG_PRINT_NUM("encoder: ", newPos);
      int newInst = newPos % 129;
      if (newInst < 0)
        newInst = 128 - newInst;
      if (newInst > 127) {
        DEBUG_PRINT_NUM("change bank: ", newInst);
        server.midiDevice->setBank(0x78); //DRUMS
        
        //previous was melodic, load drum preset
        server.loadNotes( NOTE_PRESETS_DRUMS[0] );
      }
      else {     
        DEBUG_PRINT_NUM("change bank: ", newInst); 
        
        server.midiDevice->setBank(0x79, newInst); //MELODIC
        
        if (position > 127) {
           //we switched from drums, reload the note scales        
          server.loadNotes( NOTE_PRESETS_MELODIC[0] );
        }
      }
      
      position = newPos;
    }
  }
  else { //button is depressed. cheer up, button!
     if (newPos != positionKey) {
      positionKey = newPos;
      DEBUG_PRINT_NUM("encoder key: ", positionKey);
      
      if (server.midiDevice->getBank() == 0x78) { //DRUMS
        int newInst = abs(positionKey % NOTE_PRESETS_DRUMS_LENGTH);
        server.loadNotes( NOTE_PRESETS_DRUMS[newInst] );
      }
      else { //MELODIC
        int newInst = abs(positionKey % NOTE_PRESETS_MELODIC_LENGTH);
        server.loadNotes( NOTE_PRESETS_MELODIC[newInst] );
        
      }
    }
  }
  
  lastButtonMode = buttonMode;
}

 /*
  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
 
  See file LICENSE.txt for further informations on licensing terms.

  formatted using the GNU C formatting and indenting
*/

/* 
 * TODO: use Program Control to load stored profiles from EEPROM
 */

#if !DISABLE_FIRMATA

 
#include <Servo.h>
#include <Firmata.h>

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/* analog inputs */
int analogInputsToReport = 0; // bitwise array to store pin reporting

/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent

/* pins configuration */
byte pinConfig[TOTAL_PINS];         // configuration of every pin
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int pinState[TOTAL_PINS];           // any value that has been written

/* timer variables */
unsigned long currentMillis;        // store the current value from millis()
unsigned long previousMillis;       // for comparison with currentMillis
int samplingInterval = 1;//19;          // how often to run the main loop (in ms)

Servo servos[MAX_SERVOS];

/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

void outputPort(byte portNumber, byte portValue, byte forceSend)
{
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if(forceSend || previousPINs[portNumber] != portValue) {
#if ENABLE_FIRMATA_OUTPUT    
    Firmata.sendDigitalPort(portNumber, portValue);
#endif
    previousPINs[portNumber] = portValue;
  }
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using Serial.print() */
void checkDigitalInputs(void)
{
  /* Using non-looping code allows constants to be given to readPort().
   * The compiler will apply substantial optimizations if the inputs
   * to readPort() are compile-time constants. */
  if (TOTAL_PORTS > 0 && reportPINs[0]) outputPort(0, readPort(0, portConfigInputs[0]), false);
  if (TOTAL_PORTS > 1 && reportPINs[1]) outputPort(1, readPort(1, portConfigInputs[1]), false);
  if (TOTAL_PORTS > 2 && reportPINs[2]) outputPort(2, readPort(2, portConfigInputs[2]), false);
  if (TOTAL_PORTS > 3 && reportPINs[3]) outputPort(3, readPort(3, portConfigInputs[3]), false);
  if (TOTAL_PORTS > 4 && reportPINs[4]) outputPort(4, readPort(4, portConfigInputs[4]), false);
  if (TOTAL_PORTS > 5 && reportPINs[5]) outputPort(5, readPort(5, portConfigInputs[5]), false);
  if (TOTAL_PORTS > 6 && reportPINs[6]) outputPort(6, readPort(6, portConfigInputs[6]), false);
  if (TOTAL_PORTS > 7 && reportPINs[7]) outputPort(7, readPort(7, portConfigInputs[7]), false);
  if (TOTAL_PORTS > 8 && reportPINs[8]) outputPort(8, readPort(8, portConfigInputs[8]), false);
  if (TOTAL_PORTS > 9 && reportPINs[9]) outputPort(9, readPort(9, portConfigInputs[9]), false);
  if (TOTAL_PORTS > 10 && reportPINs[10]) outputPort(10, readPort(10, portConfigInputs[10]), false);
  if (TOTAL_PORTS > 11 && reportPINs[11]) outputPort(11, readPort(11, portConfigInputs[11]), false);
  if (TOTAL_PORTS > 12 && reportPINs[12]) outputPort(12, readPort(12, portConfigInputs[12]), false);
  if (TOTAL_PORTS > 13 && reportPINs[13]) outputPort(13, readPort(13, portConfigInputs[13]), false);
  if (TOTAL_PORTS > 14 && reportPINs[14]) outputPort(14, readPort(14, portConfigInputs[14]), false);
  if (TOTAL_PORTS > 15 && reportPINs[15]) outputPort(15, readPort(15, portConfigInputs[15]), false);
}

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(byte pin, int mode)
{
  if (IS_PIN_SERVO(pin) && mode != SERVO && servos[PIN_TO_SERVO(pin)].attached()) {
    servos[PIN_TO_SERVO(pin)].detach();
  }
  if (IS_PIN_ANALOG(pin)) {
    reportAnalogCallback(PIN_TO_ANALOG(pin), mode == ANALOG ? 1 : 0); // turn on/off reporting
  }
  if (IS_PIN_DIGITAL(pin)) {
    if (mode == INPUT) {
      portConfigInputs[pin/8] |= (1 << (pin & 7));
    } else {
      portConfigInputs[pin/8] &= ~(1 << (pin & 7));
    }
  }
  pinState[pin] = 0;
  switch(mode) {
  case ANALOG:
    if (IS_PIN_ANALOG(pin)) {
      if (IS_PIN_DIGITAL(pin)) {
        pinMode(PIN_TO_DIGITAL(pin), INPUT); // disable output driver
        digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable internal pull-ups
      }
      pinConfig[pin] = ANALOG;
    }
    break;
  case INPUT:
    if (IS_PIN_DIGITAL(pin)) {
      pinMode(PIN_TO_DIGITAL(pin), INPUT); // disable output driver
      digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable internal pull-ups
      pinConfig[pin] = INPUT;
    }
    break;
  case OUTPUT:
    if (IS_PIN_DIGITAL(pin)) {
      digitalWrite(PIN_TO_DIGITAL(pin), LOW); // disable PWM
      pinMode(PIN_TO_DIGITAL(pin), OUTPUT);
      pinConfig[pin] = OUTPUT;
    }
    break;
  case PWM:
    if (IS_PIN_PWM(pin)) {
      pinMode(PIN_TO_PWM(pin), OUTPUT);
      analogWrite(PIN_TO_PWM(pin), 0);
      pinConfig[pin] = PWM;
    }
    break;
  case SERVO:
    if (IS_PIN_SERVO(pin)) {
      pinConfig[pin] = SERVO;
      if (!servos[PIN_TO_SERVO(pin)].attached()) {
          servos[PIN_TO_SERVO(pin)].attach(PIN_TO_DIGITAL(pin));
      } 
#if ENABLE_FIRMATA_OUTPUT                
      else {
  
        Firmata.sendString("Servo only on pins from 2 to 13");

      }
#endif        
    }
    break;
  case I2C:
    pinConfig[pin] = mode;
#if ENABLE_FIRMATA_OUTPUT    
    Firmata.sendString("I2C mode not yet supported");
#endif
    break;
#if ENABLE_FIRMATA_OUTPUT   
  default:   
    Firmata.sendString("Unknown pin mode"); // TODO: put error msgs in EEPROM
#endif    
  }
  // TODO: save status to EEPROM here, if changed
}

void analogWriteCallback(byte pin, int value)
{
  if (pin < TOTAL_PINS) {
    switch(pinConfig[pin]) {
    case SERVO:
      if (IS_PIN_SERVO(pin))
        servos[PIN_TO_SERVO(pin)].write(value);
        pinState[pin] = value;
      break;
    case PWM:
      if (IS_PIN_PWM(pin))
        analogWrite(PIN_TO_PWM(pin), value);
        pinState[pin] = value;
      break;
    }
  }
}

void digitalWriteCallback(byte port, int value)
{
  byte pin, lastPin, mask=1, pinWriteMask=0;

  if (port < TOTAL_PORTS) {
    // create a mask of the pins on this port that are writable.
    lastPin = port*8+8;
    if (lastPin > TOTAL_PINS) lastPin = TOTAL_PINS;
    for (pin=port*8; pin < lastPin; pin++) {
      // do not disturb non-digital pins (eg, Rx & Tx)
      if (IS_PIN_DIGITAL(pin)) {
        // only write to OUTPUT and INPUT (enables pullup)
        // do not touch pins in PWM, ANALOG, SERVO or other modes
        if (pinConfig[pin] == OUTPUT || pinConfig[pin] == INPUT) {
          pinWriteMask |= mask;
          pinState[pin] = ((byte)value & mask) ? 1 : 0;
        }
      }
      mask = mask << 1;
    }
    writePort(port, (byte)value, pinWriteMask);
  }
}


// -----------------------------------------------------------------------------
/* sets bits in a bit array (int) to toggle the reporting of the analogIns
 */
//void FirmataClass::setAnalogPinReporting(byte pin, byte state) {
//}
void reportAnalogCallback(byte analogPin, int value)
{
  if (analogPin < TOTAL_ANALOG_PINS) {
    if(value == 0) {
      analogInputsToReport = analogInputsToReport &~ (1 << analogPin);
    } else {
      analogInputsToReport = analogInputsToReport | (1 << analogPin);
    }
  }
  // TODO: save status to EEPROM here, if changed
}

void reportDigitalCallback(byte port, int value)
{
  if (port < TOTAL_PORTS) {
    reportPINs[port] = (byte)value;
  }
  // do not disable analog reporting on these 8 pins, to allow some
  // pins used for digital, others analog.  Instead, allow both types
  // of reporting to be enabled, but check if the pin is configured
  // as analog when sampling the analog inputs.  Likewise, while
  // scanning digital pins, portConfigInputs will mask off values from any
  // pins configured as analog
}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv)
{
  switch(command) {
  case SERVO_CONFIG:
    if(argc > 4) {
      // these vars are here for clarity, they'll optimized away by the compiler
      byte pin = argv[0];
      int minPulse = argv[1] + (argv[2] << 7);
      int maxPulse = argv[3] + (argv[4] << 7);

      if (IS_PIN_SERVO(pin)) {
        // servos are pins from 2 to 13, so offset for array
        if (servos[PIN_TO_SERVO(pin)].attached())
          servos[PIN_TO_SERVO(pin)].detach();
        servos[PIN_TO_SERVO(pin)].attach(PIN_TO_DIGITAL(pin), minPulse, maxPulse);
        setPinModeCallback(pin, SERVO);
      }
    }
    break;
  case SAMPLING_INTERVAL:
    if (argc > 1)
      samplingInterval = argv[0] + (argv[1] << 7);
#if ENABLE_FIRMATA_OUTPUT    
    else
      Firmata.sendString("Not enough data");
#endif      
    break;
  case EXTENDED_ANALOG:
    if (argc > 1) {
      int val = argv[1];
      if (argc > 2) val |= (argv[2] << 7);
      if (argc > 3) val |= (argv[3] << 14);
      analogWriteCallback(argv[0], val);
    }
    break;
  case CAPABILITY_QUERY:
    Serial.write(START_SYSEX);
    Serial.write(CAPABILITY_RESPONSE);
    for (byte pin=0; pin < TOTAL_PINS; pin++) {
      if (IS_PIN_DIGITAL(pin)) {
        Serial.write((byte)INPUT);
        Serial.write(1);
        Serial.write((byte)OUTPUT);
        Serial.write(1);
      }
      if (IS_PIN_ANALOG(pin)) {
        Serial.write(ANALOG);
        Serial.write(10);
      }
      if (IS_PIN_PWM(pin)) {
        Serial.write(PWM);
        Serial.write(8);
      }
      if (IS_PIN_SERVO(pin)) {
        Serial.write(SERVO);
        Serial.write(14);
      }
      Serial.write(127);
    }
    Serial.write(END_SYSEX);
    break;
  case PIN_STATE_QUERY:
    if (argc > 0) {
      byte pin=argv[0];
      Serial.write(START_SYSEX);
      Serial.write(PIN_STATE_RESPONSE);
      Serial.write(pin);
      if (pin < TOTAL_PINS) {
        Serial.write((byte)pinConfig[pin]);
	Serial.write((byte)pinState[pin] & 0x7F);
	if (pinState[pin] & 0xFF80) Serial.write((byte)(pinState[pin] >> 7) & 0x7F);
	if (pinState[pin] & 0xC000) Serial.write((byte)(pinState[pin] >> 14) & 0x7F);
      }
      Serial.write(END_SYSEX);
    }
    break;
  case ANALOG_MAPPING_QUERY:
    Serial.write(START_SYSEX);
    Serial.write(ANALOG_MAPPING_RESPONSE);
    for (byte pin=0; pin < TOTAL_PINS; pin++) {
      Serial.write(IS_PIN_ANALOG(pin) ? PIN_TO_ANALOG(pin) : 127);
    }
    Serial.write(END_SYSEX);
    break;
  }
}


/*==============================================================================
 * SETUP()
 *============================================================================*/
void setup() 
{
  byte i;

#if ENABLE_FIRMATA_OUTPUT
  Firmata.setFirmwareVersion(2, 2);

  Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
  Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
  Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);
  Firmata.attach(SET_PIN_MODE, setPinModeCallback);
  Firmata.attach(START_SYSEX, sysexCallback);
#endif
  // TODO: load state from EEPROM here

  /* these are initialized to zero by the compiler startup code
  for (i=0; i < TOTAL_PORTS; i++) {
    reportPINs[i] = false;
    portConfigInputs[i] = 0;
    previousPINs[i] = 0;
  }
  */
  
  
  
  
  
   
  
  for (i = FIRST_USABLE_PIN; i < TOTAL_PINS; i++) {
    if (IS_PIN_ANALOG(i)) {
      // turns off pullup, configures everything
      setPinModeCallback(i, ANALOG);
    } else {
      // sets the output to 0, configures portConfigInputs
      switch (i) {
        case ENCODER_PIN_1:
        case ENCODER_PIN_2:
        case ENCODER_MODE_SWITCH_PIN:
          setPinModeCallback(i, INPUT);
          break;
#if !IS_DUE        
        default:
          setPinModeCallback(i, OUTPUT);
#endif
      }
    } 
  }
  
  
  
  // by defult, do not report any analog inputs
  //report all by default
  //analogInputsToReport = 0;
#if ENABLE_FIRMATA_OUTPUT
  // we can't begin on the same Serial twice - MIDIDevice also uses Serial since it's the hardware one.
  
  Firmata.begin(BAUD_RATE);
  
#else 

  Serial.begin(BAUD_RATE);
#endif

  // send digital inputs to set the initial state on the host computer,
  // since once in the loop(), this firmware will only send on change 
  for (i=0; i < TOTAL_PORTS; i++) {
    outputPort(i, readPort(i, portConfigInputs[i]), true);
  }

  setupKnobs();
    
  setupServer();

  DEBUG_PRINT("END SETUP")
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop() 
{
  byte pin, analogPin;

  checkKnobs();
  

#if ENABLE_FIRMATA_OUTPUT   
  /* DIGITALREAD - as fast as possible, check for changes and output them to the
   * FTDI buffer using Serial.print()  */
  checkDigitalInputs();  

  /* SERIALREAD - processing incoming messagse as soon as possible, while still
   * checking digital inputs.  */
  while(Firmata.available())
    Firmata.processInput();
#endif 

  /* SEND FTDI WRITE BUFFER - make sure that the FTDI buffer doesn't go over
   * 60 bytes. use a timer to sending an event character every 4 ms to
   * trigger the buffer to dump. */

  currentMillis = millis();
  if (currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;
    /* ANALOGREAD - do all analogReads() at the configured sampling interval */
    for(pin=0; pin<TOTAL_PINS; pin++) {
      if (IS_PIN_ANALOG(pin) && pinConfig[pin] == ANALOG) {
        analogPin = PIN_TO_ANALOG(pin);
        if (analogInputsToReport & (1 << analogPin)) {
          int val = analogRead(analogPin);
          
#if !ENABLE_TEST
          server.readPin(analogPin, val);
#endif          

#if ENABLE_FIRMATA_OUTPUT          
          //Firmata.sendAnalog(analogPin, val);
#endif          
        }
      }
    }
  }

#if ENABLE_LOOPER  
  // loop that shit! 
  server.tick();
#endif
  
#if ENABLE_TEST
  testUpdate();
  return;
#endif
}

//end DISABLE_FIRMATA
#endif

void setupServer() {
    server.init();

    // load good drums preset to start
    server.loadNotes( NOTE_PRESETS_DRUMS[0] );

    // must wait a second for the MIDI device to boot up before it accepts our messages
    delay(1000);

    server.midiDevice->setBank(0x78); //DRUMS
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
  server.readPin(testPin, val);
}
#endif


//try without firmata
#if DISABLE_FIRMATA
byte resetMIDI = 4; //Tied to VS1053 Reset line
void setup() {
  Serial.begin(BAUD_RATE);// was 57600);

  //Setup soft serial for MIDI control
 // mySerial.begin(31250);

  //Reset the VS1053
/*
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);

  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)

  talkMIDI(0xB0, 0, 0x78); //Select the bank of really fun sounds

  //For this bank 0x78, the instrument does not matter, only the note
  talkMIDI(0xC0, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
*/

  DEBUG_PRINT("Press a letter and press enter!");
    
  setupKnobs();  
    
  setupServer();
}
int thePin = 0;
void loop() {
  
#if ENABLE_TEST
  testUpdate();
  return;
#endif

  checkKnobs();
  while(Serial.available() == 0) {
    checkKnobs();

    server.tick();
  }

  int note = Serial.read();  
  //Good sounding notes range from 27 to 87
  //So let's take the letter and range it to 27
  note -= 'a';
  note += 57;

  DEBUG_PRINT_NUM("Note=", note);

  DEBUG_PRINT("Done NOTE");

  //For this bank 0x78, the instrument does not matter, only the note
  //noteOn(0, note, 60);

  thePin = (thePin + 1) % 6;

  if (note < 60)
    server.readPin(thePin, 0);
  else if (note < 75)
    server.readPin(thePin, note);
  else
    server.readPin(thePin, 1024);
  //Note will dissapate automatically
  
  
  DEBUG_PRINT("Done server.readPin");
}

#endif
