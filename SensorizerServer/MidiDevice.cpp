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
 /* NEEDED??

 */
 
#include "MidiDevice.h"

#include "looper/EventLooper.h"

#ifdef IS_ARDUINO_SHIM
    #include "ofxMidi.h"


ofxMidiOut midiOut;

#endif

//SoftwareSerial mySerial(2, 3);

MidiDevice::MidiDevice() {

    
#ifdef IS_ARDUINO_SHIM
	// print the available output ports to the console
	midiOut.listPorts(); // via instance
	//ofxMidiOut::listPorts(); // via static too
	
	// connect
	midiOut.openPort(0);	// by number
	//midiOut.openPort("IAC Driver Pure Data In");	// by name

#else
    #if USE_HARDWARE_SERIAL
        // Ok, the Serial object is not actually the HardwareSerial class, it's something else.
        //this->mySerial = &Serial;
        // do some tricky de-re-referencing to get around it.
        #if IS_DRUMPANTS
	        #define mySerial (&Serial2)
		#else
			#define mySerial (&Serial)
		#endif
    #else
        this->mySerial = new SoftwareSerial(2, 3, false); //Soft TX on 3, we don't use RX in this code
    #endif
#endif
	
	this->bank = 0x78; // start with DRUMS!
	this->instrument = 0; // was 30 for piano

	//manually init array
	for (int i = 0; i < LISTENERS_LENGTH; i++) {
		listeners[i] = NULL;
	}
}

MidiDevice::~MidiDevice() {
	//delete this->mySerial;
	//
	this->destroyListeners();
}

void MidiDevice::setup() {
	//Serial.println("Initializing MidiDevice");

	//Serial.begin(57600);
	
	//Setup soft serial for MIDI control
	mySerial->begin(BAUD_RATE_MIDI);
	

#ifndef IS_ARDUINO_SHIM
	//Reset the VS1053
	pinMode(RESET_MIDI_PIN, OUTPUT);
	digitalWrite(RESET_MIDI_PIN, LOW);
	delay(100);
	digitalWrite(RESET_MIDI_PIN, HIGH);
	delay(100);
#endif
    
	
	this->setVolume(0, 120); //0xB0 is channel message, set channel volume to near max (127)

	// give us some reverb!
	this->talkMIDI(0xB0, 0x0c, 0xEF); // REVERB decay
	this->talkMIDI(0xB0, 0x5b, 0xA3); // REVERB level
	
	this->setBank(0, this->bank); //Select the bank of really fun sounds
	
	//For this bank 0x78, the instrument does not matter, only the note
	//setInstrument(instrument); //Set instrument number. 0xC0 is a 1 data byte command
	
	/*** PITCH BEND ***/
	// from http://www.vsdsp-forum.com/phpbb/viewtopic.php?f=9&t=465
	// indicate we intend to set pitch bend limites
	talkMIDI(0xB0|0, 0x65, 00);
	talkMIDI(0xB0|0, 0x64, 00);

	// set the semitone limits
	talkMIDI(0xB0|0, 0x06, 24); // 24 should give me a full octave of pitch-bending up and down!!!

	// set the cents limits (fine-tuning)
	//talkMIDI(0xB0|channel, 0x26, 00);

	// reset the RPN controller
	talkMIDI(0xB0|0, 0x65, 127);
	talkMIDI(0xB0|0, 0x64, 127);
	/*** END PITCH BEND ***/
	
	//Serial.println("MidiDevice now setup");
}

void MidiDevice::setBank(byte channel, byte bank, byte instrument) {
	DEBUG_PRINT_NUM("setBank: ", bank)
	talkMIDI((0xB0 | channel), 0, bank); //Select the bank of really fun sounds
	
	this->bank = bank;
	
	setInstrument(channel, instrument);
}
byte MidiDevice::getBank() {
	return bank;
}

void MidiDevice::setInstrument(byte channel, byte inst) {
	DEBUG_PRINT_NUM("setInstrument: ", inst)
	talkMIDI((0xC0 | channel), inst, 0); //Set instrument number. 0xC0 is a 1 data byte command
	
	this->instrument = inst;
}
byte MidiDevice::getInstrument() {
	return this->instrument;
}

void MidiDevice::setVolume(byte channel, byte vol) {
	talkMIDI((0xB0 | channel), 0x07, vol); 
}


//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void MidiDevice::noteOn(byte channel, byte note, byte attack_velocity) {
	//DEBUG_PRINT_NUMS("MIDI noteOn: ", note, attack_velocity);
  
	talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void MidiDevice::noteOff(byte channel, byte note, byte release_velocity) {
	//DEBUG_PRINT_NUMS("MIDI noteOff: ", note, release_velocity);
 	
 	talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
// if isSilent is true, it will not alert the listener.
void MidiDevice::talkMIDI(byte cmd, byte data1, byte data2, bool isSilent) {

    //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
    //(sort of: http://253.ccarh.org/handout/midiprotocol/)
    //if( (cmd & 0xF0) <= 0xB0)
    bool hasSecondArg = ((cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) == 0xE0);

#ifdef IS_ARDUINO_SHIM
    midiOut << cmd << data1;
    
    if (hasSecondArg)
        midiOut << data2;
    
#else
	digitalWrite(LED_PIN, HIGH);
	mySerial->write(cmd);//print(cmd, BYTE);
	mySerial->write(data1);//print(data1, BYTE);

	if (hasSecondArg)
		mySerial->write(data2);//print(data2, BYTE);

  	digitalWrite(LED_PIN, LOW);
#endif

  if (!isSilent) {

  	for (int i = 0; i < LISTENERS_LENGTH; i++) {
	  	if (this->listeners[i] != NULL) {
			this->listeners[i]->onSendOutput(cmd, data1, data2);
	  	}
	}
  }

}


void MidiDevice::note(bool isOn, int channel, int note, int velocity) {
	// i assumed the truncation from int to byte would take care of this.
	// ProTip: don't trust C++ to do anything reasonable.
	channel = min(channel, 127);
	note = min(note, 127);
	velocity = min(velocity, 127);


	if (isOn)
		noteOn((byte)channel, (byte)note, (byte)velocity);
	else
		noteOff((byte)channel, (byte)note, 0); //TODO: velocity or 0?
}

void MidiDevice::cc(int channel, int num, int velocity) {
	talkMIDI( (0xB0 | channel), num, velocity);
}

void MidiDevice::bendPitch(int channel, int velocity) {
	talkMIDI( (0xE0 | channel), 0, velocity);
}

/*** for looper listeners.

Silently fails if you try to add more than 2 listeners! ***/
void MidiDevice::addListener(MidiListener* l) {

	for (int i = 0; i < LISTENERS_LENGTH; i++) {
	  	if (this->listeners[i] != NULL) {
			this->listeners[i] = l;
			break;
	  	}
	}
}

void MidiDevice::destroyListeners() {
	for (int i = 0; i < LISTENERS_LENGTH; i++) {
	  	if (this->listeners[i] != NULL) {
			delete this->listeners[i];

			this->listeners[i] = NULL;
	  	}
	}
}




