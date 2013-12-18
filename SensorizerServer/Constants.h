#ifndef SensorizerConstants_h
#define SensorizerConstants_h


/******* hardware version type presets ********/

// for the beta DrumPants PCB:
// NOTE: all beta presets are >= 10 and it is used to set other constants!
#define PRESET_BETA 10
#define PRESET_BETA_RYAN 11
#define PRESET_BETA_STRETCHY 12
#define PRESET_BETA_NEIL_PEART 14

// for the earlier prototypes
#define PRESET_NEON_GREEN_STRING 3
#define PRESET_VJACKET_LEATHER 2
#define PRESET_JEANS 1

#define PRESET PRESET_BETA_NEIL_PEART

/******* END hardware version type presets ********/



#define BLE_TYPE_NONE 0
#define BLE_TYPE_BLUEGIGA 1
#define BLE_TYPE_REDBEARLABS 2




#if PRESET == PRESET_VJACKET_LEATHER

  // to use BLE:
  //#define BLE_TYPE BLE_TYPE_BLUEGIGA
  //#define USE_HARDWARE_SERIAL 1

  // to use MIDI shield:
  #define BLE_TYPE BLE_TYPE_NONE
  #define USE_HARDWARE_SERIAL 0

#elif PRESET == PRESET_NEON_GREEN_STRING

  #define BLE_TYPE BLE_TYPE_REDBEARLABS
  #define USE_HARDWARE_SERIAL 1

#else
  #define BLE_TYPE BLE_TYPE_NONE
#endif

// can't use Firmata on USB and also debug print on USB:
#define ENABLE_FIRMATA (!ENABLE_DEBUG_PRINTING)

//uncomment this to set serial baud at bluetooth rate. otherwise, USB rate.
//# define IS_BLUETOOTH

#if PRESET >= PRESET_BETA
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


// the default num of ticks before a note retriggers
#define DEFAULT_RETRIGGER_THRESHOLD 50


#if IS_DUE  
  #define USE_HARDWARE_SERIAL 1


  // start off on the middle since our sensors only have 6 still.
  #if PRESET == PRESET_BETA_RYAN
    #define ANALOG_PIN_START 2
    #define ANALOG_PIN_END 8
  #elif PRESET == PRESET_BETA_STRETCHY
    #define ANALOG_PIN_START 2
    #define ANALOG_PIN_END 11
  #elif PRESET == PRESET_BETA_NEIL_PEART
    #define ANALOG_PIN_START 0
    #define ANALOG_PIN_END 11
  #else
    #define ANALOG_PIN_START 1
    #define ANALOG_PIN_END 11
  #endif

  // TODO: these need to be updated for DUE (MORE NOTES!). hacking for now
  #define NOTE_PRESETS_ELEMENT_LENGTH 9

#else

  // normal arduinos have 6 analog ins
  #define ANALOG_PIN_START 0
  #define ANALOG_PIN_END 5

  #define NOTE_PRESETS_ELEMENT_LENGTH 6

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