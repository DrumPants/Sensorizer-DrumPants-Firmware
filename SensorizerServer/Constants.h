#ifndef SensorizerConstants_h
#define SensorizerConstants_h


/******* hardware version type presets ********/

// for the beta DrumPants PCB:
#define PRESET_BETA 10
// for the earlier prototypes
#define PRESET_NEON_GREEN_STRING 3
#define PRESET_VJACKET_LEATHER 2
#define PRESET_JEANS 1

#define PRESET PRESET_VJACKET_LEATHER

/******* END hardware version type presets ********/



#define BLE_TYPE_NONE 0
#define BLE_TYPE_BLUEGIGA 1
#define BLE_TYPE_REDBEARLABS 2




#if PRESET == PRESET_VJACKET_LEATHER

  // to use BLE:
  #define BLE_TYPE BLE_TYPE_BLUEGIGA
  #define USE_HARDWARE_SERIAL 1

  // to use MIDI shield:
  //#define BLE_TYPE BLE_TYPE_NONE
  //#define USE_HARDWARE_SERIAL 0

#elif PRESET == PRESET_NEON_GREEN_STRING

  #define BLE_TYPE BLE_TYPE_REDBEARLABS
  #define USE_HARDWARE_SERIAL 1

#else
  #define BLE_TYPE BLE_TYPE_NONE
#endif

//uncomment this to set serial baud at bluetooth rate. otherwise, USB rate.
//# define IS_BLUETOOTH

#if PRESET == PRESET_BETA
  #define IS_DUE 1
  #define IS_DRUMPANTS 1
#else
  #define IS_DUE 0
  #define IS_DRUMPANTS 0
#endif

#define IS_BLE 0

#define ENABLE_TEST 0

#define BAUD_RATE_USB 57600
#define BAUD_RATE_BLUETOOTH 115200
#define BAUD_RATE_BLUETOOTH_LE 19200

// for the MIDI chip in MidiDevice class
#if BLE_TYPE == BLE_TYPE_BLUEGIGA
  #define BAUD_RATE_MIDI 19200
#elif BLE_TYPE == BLE_TYPE_REDBEARLABS
  #define BAUD_RATE_MIDI BAUD_RATE_USB
#else
  #define BAUD_RATE_MIDI 31250
#endif

// the channel to send midi notes
#define MIDI_CHANNEL 0

#define ENABLE_LOOPER 0

#if IS_DUE
  #define FIRST_USABLE_PIN 2
  
  #define USE_HARDWARE_SERIAL 1
#else
  #define FIRST_USABLE_PIN 0
#endif

#if IS_BLE
  //#define BAUD_RATE BAUD_RATE_BLUETOOTH_LE
  #define BAUD_RATE BAUD_RATE_USB
  
  #define USE_HARDWARE_SERIAL 0
  
  // for testing:
  #define BAUD_RATE_MIDI BAUD_RATE_BLUETOOTH_LE
  
#elif IS_BLUETOOTH
  #define BAUD_RATE BAUD_RATE_BLUETOOTH
#else
  #define BAUD_RATE BAUD_RATE_USB
#endif


#endif