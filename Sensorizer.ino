#include <Constants.h>  
#include <Debug.h>

#if !USE_HARDWARE_SERIAL
#include <SoftwareSerial.h>
#endif 

#include <SevenSegmentController.h>

#include <SensorizerServer.h>  


#if ENABLE_FIRMATA
  #include <Firmata.h>

  // how many ticks it takes before sending a Firmata update. 
  // this needs to be throttled because otherwise we overload the Serial buffer and shit freezes.
  #define FIRMATA_UPDATE_RATE_THROTTLE 20

// how many ticks it's been since the last Firmata update. 
int firmataThrottleCount = 0;  
#endif


#if IS_DUE 
  #define ENCODER_OPTIMIZE_INTERRUPTS
#else
  #define ENCODER_DO_NOT_USE_INTERRUPTS
#endif
#include <Encoder.h>

#include "Knobs.h"

// for intro sound
#include "IntroSounds.h"

SensorizerServer* server;

Knobs* knobs;




void setupServer() {
    // must wait a bit for the MIDI device to boot up before it accepts our messages
    delay(500);
    
    
    server = new SensorizerServer();
    server->init();

    
    // the setup will load good drums preset to start
    knobs = new Knobs();
    knobs->setup(server);
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
  //DEBUG_PRINT_SETUP

#if IS_DUE


  #ifdef BLE_RESET_PIN

  // broadcom has reset line attached, make sure it's not reseting!
  // active-low.

  // first, set programming HCI UART to low, because the BLE will just wait to be programmed 
  // if it starts up with the TX line high.
  // TODO: Not sure what setting the TX pin will do for the actual communication later. Will the UART class successfully regain control of the TX line?
#define BLE_PROGRAMMING_TX_PIN 1 
  pinMode(BLE_PROGRAMMING_TX_PIN, OUTPUT);
  digitalWrite(BLE_PROGRAMMING_TX_PIN, LOW); 

  // reset the BLE
  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_RESET_PIN, LOW); 
  delay(2); // 20uS to reset

  pinMode(BLE_RESET_PIN, OUTPUT);
  digitalWrite(BLE_RESET_PIN, HIGH); 

  delay(6); // ~5ms to warm up and boot

  // TODO: uncomment this to implement reprograming the BLE firmware via its HCI UART
  Serial.begin(115200);

  #endif

  // this is needed for debug printing on native USB port
  SerialUSB.begin(57600);

  #if ENABLE_FIRMATA
  Firmata.setFirmwareVersion(0, 1);
  Firmata.begin(SerialUSB);
  #endif
#else
  Serial.begin(BAUD_RATE);
#endif


  DEBUG_PRINT("SETUPSERVER")
  setupServer();

  DEBUG_PRINT("PLAYING INTRO SOUNDS")
  
  playIntro(server);


  DEBUG_PRINT("END SETUP")
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop() 
{
  byte analogPin;

 // Serial.println("loop ser");
 // SerialUSB.println("loop usb");

  //checkKnobs();
  knobs->check();
  
#if ENABLE_FIRMATA
  // while(Firmata.available()) {
  //   Firmata.processInput();
  // }
#endif  

  /* SEND FTDI WRITE BUFFER - make sure that the FTDI buffer doesn't go over
   * 60 bytes. use a timer to sending an event character every 4 ms to
   * trigger the buffer to dump. */

  currentMillis = millis();
  if (currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;

    /* ANALOGREAD - do all analogReads() at the configured sampling interval */
    for(analogPin = ANALOG_PIN_START; analogPin <= ANALOG_PIN_END; analogPin++) {
        byte pinIdx = analogPin - ANALOG_PIN_START;
        int val = analogRead(analogPin);
          
        //DEBUG_PRINT_NUM("check pin ", pinIdx);  
#if !ENABLE_TEST
        server->readPin(pinIdx, val);
#endif          

#if ENABLE_FIRMATA
      if (firmataThrottleCount == FIRMATA_UPDATE_RATE_THROTTLE) {
        Firmata.sendAnalog(analogPin, val); 
      }
#endif 
    }

#if ENABLE_FIRMATA
    if (++firmataThrottleCount > FIRMATA_UPDATE_RATE_THROTTLE) {
        firmataThrottleCount = 0;
    }
#endif
  }


  // relay all debug messages from BLE 
#if ENABLE_DEBUG_PRINTING && defined(BLE_RESET_PIN)

  while(Serial.available()) {
    SerialUSB.write(Serial.read());
  }

#endif


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
