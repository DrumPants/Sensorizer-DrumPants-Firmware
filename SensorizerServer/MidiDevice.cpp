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

//SoftwareSerial mySerial(2, 3);

MidiDevice::MidiDevice() {

#if USE_HARDWARE_SERIAL
	// Ok, the Serial object is not actually the HardwareSerial class, it's something else.
	//this->mySerial = &Serial;
	// do some tricky de-re-referencing to get around it.
	#define mySerial (&Serial)
#else
	this->mySerial = new SoftwareSerial(2, 3, false); //Soft TX on 3, we don't use RX in this code
#endif
	
	this->bank = 0x78; // start with DRUMS!
	this->instrument = 0; // was 30 for piano

	this->listener = NULL;
}

MidiDevice::~MidiDevice() {
	//delete this->mySerial;
}

void MidiDevice::setup() {
	//Serial.println("Initializing MidiDevice");

	//Serial.begin(57600);
	
	//Setup soft serial for MIDI control
	mySerial->begin(BAUD_RATE_MIDI);


	//Reset the VS1053
	pinMode(RESET_MIDI_PIN, OUTPUT);
	digitalWrite(RESET_MIDI_PIN, LOW);
	delay(100);
	digitalWrite(RESET_MIDI_PIN, HIGH);
	delay(100);
	
	talkMIDI(0xB0, 0x07, 126); //0xB0 is channel message, set channel volume to near max (127)
	
	setBank(this->bank); //Select the bank of really fun sounds
	
	//For this bank 0x78, the instrument does not matter, only the note
	//setInstrument(instrument); //Set instrument number. 0xC0 is a 1 data byte command
	
	
	//Serial.println("MidiDevice now setup");
}

void MidiDevice::setBank(byte bank, byte instrument) {
	DEBUG_PRINT_NUM("setBank: ", bank)
	talkMIDI(0xB0, 0, bank); //Select the bank of really fun sounds
	
	this->bank = bank;
	
	setInstrument(instrument);
}
byte MidiDevice::getBank() {
	return bank;
}

void MidiDevice::setInstrument(byte inst) {
	DEBUG_PRINT_NUM("setInstrument: ", inst)
	talkMIDI(0xC0, inst, 0); //Set instrument number. 0xC0 is a 1 data byte command
	
	this->instrument = inst;
}
byte MidiDevice::getInstrument() {
	return this->instrument;
}

/*
void loop() {
  while(Serial.available() == 0) ;

  int note = Serial.read();
  //Good sounding notes range from 27 to 87
  //So let's take the letter and range it to 27
  note -= 'a';
  note += 57;

  Serial.print("Note=");
  Serial.println(note, DEC);

  //For this bank 0x78, the instrument does not matter, only the note
  noteOn(0, note, 60);

  //Note will dissapate automatically
}
*/

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void MidiDevice::noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void MidiDevice::noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
// if isSilent is true, it will not alert the listener.
void MidiDevice::talkMIDI(byte cmd, byte data1, byte data2, bool isSilent) {
  digitalWrite(LED_PIN, HIGH);
  mySerial->write(cmd);//print(cmd, BYTE);
  mySerial->write(data1);//print(data1, BYTE);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  //if( (cmd & 0xF0) <= 0xB0)
  if( (cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) == 0xE0)
    mySerial->write(data2);//print(data2, BYTE);

  digitalWrite(LED_PIN, LOW);

  if (!isSilent && this->listener != NULL) {
	this->listener->onSendOutput(cmd, data1, data2);
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



/*** for looper listeners ***/
void MidiDevice::setListener(EventLooper* l) {
	this->listener = l;
}





