#ifndef SensorizerConstants_h
#define SensorizerConstants_h

//uncomment this to set serial baud at bluetooth rate. otherwise, USB rate.
//# define IS_BLUETOOTH

#define IS_DUE 0
#define IS_BLE 0

#define ENABLE_TEST 0

#define BAUD_RATE_USB 57600
#define BAUD_RATE_BLUETOOTH 115200
#define BAUD_RATE_BLUETOOTH_LE 19200

// for the MIDI chip in MidiDevice class
#define BAUD_RATE_MIDI 31250

// the channel to send midi notes
#define MIDI_CHANNEL 0

#define ENABLE_LOOPER 0

#if IS_DUE
  #define FIRST_USABLE_PIN 2
  // NOTE: this requires installing Firmata 2.3.5 into the Arduino IDE (see http://firmata.org/wiki/Main_Page#Arduino_Due )
  #define DISABLE_FIRMATA 0
  
  // we can't begin on the same Serial twice - MIDIDevice also uses Serial since it's the hardware one.
  // so for now we disable Firmata.begin()
  #define ENABLE_FIRMATA_OUTPUT 0
  
  #define USE_HARDWARE_SERIAL 1
#else
  #define FIRST_USABLE_PIN 0
  #define DISABLE_FIRMATA 0
#endif

#if IS_BLE
  //#define BAUD_RATE BAUD_RATE_BLUETOOTH_LE
  #define BAUD_RATE BAUD_RATE_USB
  
  #define ENABLE_FIRMATA_OUTPUT 0
  
  #define USE_HARDWARE_SERIAL 0
  
  #define DISABLE_FIRMATA 1
  
  // for testing:
  #define BAUD_RATE_MIDI BAUD_RATE_BLUETOOTH_LE
  
#elif IS_BLUETOOTH
  #define BAUD_RATE BAUD_RATE_BLUETOOTH
#else
  #define BAUD_RATE BAUD_RATE_USB
#endif


#endif