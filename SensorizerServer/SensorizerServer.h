/*
 2-12-2011
 Spark Fun Electronics 2011
 Nathan Seidle
 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This code works with the VS1053 Breakout Board and controls the VS1053 in what is called Real Time MIDI mode. 
 To get the VS1053 into RT MIDI mode, power up the VS1053 breakout board with GPIO0 tied low, GPIO1 tied high.
 
 I use the NewSoftSerial library to send out the MIDI serial at 31250bps. This allows me to print regular messages
 for debugging to the terminal window. This helped me out a ton.
 
 5V : VS1053 VCC
 GND : VS1053 GND
 D3 (SoftSerial TX) : VS1053 RX
 D4 : VS1053 RESET
 
 Attach a headphone breakout board to the VS1053:
 VS1053 LEFT : TSH
 VS1053 RIGHT : RSH
 VS1053 GBUF : GND
 
 When in the drum bank (0x78), there are not different instruments, only different notes.
 To play the different sounds, select an instrument # like 5, then play notes 27 to 87.
 
 To play "Sticks" (31):
 talkMIDI(0xB0, 0, 0x78); //Bank select: drums
 talkMIDI(0xC0, 5, 0); //Set instrument number
 //Play note on channel 1 (0x90), some note value (note), middle velocity (60):
 noteOn(0, 31, 60);
 
 */
#ifndef SensorizerServer_h
#define SensorizerServer_h

#include <Arduino.h>
/*
# include <HardwareSerial.h>
# include <WConstants.h> 
*/
#include <SoftwareSerial.h>


//Tied to VS1053 Reset line
#define RESET_MIDI_PIN 4

//MIDI traffic inidicator
#define LED_PIN 13

class SensorizerServer {

private: 

	SoftwareSerial* mySerial;//(2, 3); //Soft TX on 3, we don't use RX in this code


	//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
	void talkMIDI(byte cmd, byte data1, byte data2);
	
	
public:
	
	static const double SENSOR_VALUE_NULL = -1.0;
	
	SensorizerServer();
	~SensorizerServer();
	
	//sets us up the server for playing notes
	void setup();
	
	//Send a MIDI note-on message.  Like pressing a piano key
	//channel ranges from 0-15
	void noteOn(byte channel, byte note, byte attack_velocity);
	
	//Send a MIDI note-off message.  Like releasing a piano key
	void noteOff(byte channel, byte note, byte release_velocity);

};

#endif


