 /* NEEDED??

 */
 
#include "SensorizerServer.h"

#include "filters/OneHitDetector.h"
#include "filters/NullableOutputFilter.h"


SensorizerServer::SensorizerServer() {
	//manually init array
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		sensorInputs[i] = NULL;
	}

	this->midiDevice = new MidiDevice(); 

#if IS_DRUMPANTS
	// make the BLE repeat all messages we're sending
	MidiRepeater* repeater = new TranslatingMidiRepeater(this);
	this->midiDevice->addListener(repeater);

#endif

	// metronome off by default.
	metronome.timePerTick = 0;
	metronomeTimeSignature = 4;
	metronomeCurBeat = 0;

#if ENABLE_LOOPER
	//set up looper
	this->looper = new EventLooper();
	this->looper->setOutputDevice(this->midiDevice);

	// record and loop each output
	this->midiDevice->addListener(this->looper);
#endif
}

SensorizerServer::~SensorizerServer() {
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL)
			delete sensorInputs[i];
	}

// not needed anymore: this is handled in midiDevice destructor
// #if ENABLE_LOOPER
// 	delete this->looper;
// #endif

	delete this->midiDevice;
}


void SensorizerServer::init() {
	this->midiDevice->setup();
	
	// must do this after midiDevice is set up other wise it crashes on Due
	this->loadPreset();

	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL)
			sensorInputs[i]->init();
	}

	metro.timePerTick = TIME_PER_TICK;
}

void SensorizerServer::tick() {
	metronomeTick();

	int curTime = metro.hasTicked();

	if (curTime == 0)
		return;

#if ENABLE_LOOPER
	this->looper->tick(curTime);
#endif
}

void SensorizerServer::metronomeTick() {
	int curTime = metronome.hasTicked();

	if (curTime == 0)
		return;

	int note = METRONOME_NOTE;
	int lastNote = METRONOME_NOTE;
	int vel = 80;
	metronomeCurBeat++;
	if (metronomeCurBeat >= metronomeTimeSignature) {
		metronomeCurBeat = 0;
		note = METRONOME_NOTE_START;
		vel = 127;
	}
	else if (metronomeCurBeat == 1) {
		lastNote = METRONOME_NOTE_START;
	}

	midiDevice->noteOff(METRONOME_MIDI_CHANNEL, lastNote, 00);
	midiDevice->noteOn(METRONOME_MIDI_CHANNEL, note, vel);
}

void SensorizerServer::startMetronome(int bpm, int divisions) {
	metronomeTimeSignature = divisions;
	metronomeCurBeat = 0;

	// metronome was off before, send starting note.
	if (metronome.timePerTick == 0) {
		// select drums
	    midiDevice->setBank(METRONOME_MIDI_CHANNEL, 0x78); 

		// start the cycle so theres an on for every off.
		midiDevice->noteOn(METRONOME_MIDI_CHANNEL, METRONOME_NOTE_START, 80);
	}

	metronome.timePerTick = (1000.0f / ((float)bpm / 60.0f));
}

void SensorizerServer::stopMetronome() {
	startMetronome(0);

	// finish the cycle so theres an on for every off
	if (metronomeCurBeat == 0) {
		midiDevice->noteOff(METRONOME_MIDI_CHANNEL, METRONOME_NOTE_START, 00);
	}
	else {
		midiDevice->noteOff(METRONOME_MIDI_CHANNEL, METRONOME_NOTE, 00);	
	}
}

int SensorizerServer::getMetronomeBPM(int divisions) {
	if (metronome.timePerTick == 0) return 0;

	return (1000.0f / (float)metronome.timePerTick) * 60.0f;
}

//reads all input devices values into the sensorInput objects.
//also sends all values from sensor inputs.
/*
void SensorizerServer::readAll() {
	//if (arduino != null) {
		//arduino bindings
		for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
	//		if (sensorInputs[i].deviceName == "arduino") {
				//float v = (float)arduino.analogRead(sensorInputs[i].getDeviceInputIdx()) / 1023.0f;
				//float v = arduino.read(sensorInputs[i].getDeviceInputIdx());
				float v = analogRead(i);
				
				//if (v > 0)
				//	Debug.print(Debug.DEBUG, "ardunio input " + i + ": " + v);		
				
				sensorInputs[i]->setValue(v);
				sensorInputs[i]->send();
	//		}
		}
	//}
}
*/

void SensorizerServer::readPin(int pinIdx, int pinValue) {
	if (pinIdx >= SENSOR_INPUTS_LENGTH) {
		DEBUG_PRINT_NUM("readPin INVALID PIN", pinIdx);	
		return;
	}

	if (sensorInputs[pinIdx] == NULL)
		return;

	float v = pinValue / 1024.0;

//DEBUG_PRINT_NUM("readPin pin: ", pinIdx)
//DEBUG_PRINT_NUM("readPin val: ", v)
  
	sensorInputs[pinIdx]->setValue(v);
	sensorInputs[pinIdx]->send();
}

void SensorizerServer::loadPreset() {
	int i = 0;
	SensorOutput* s;
	MidiMapping* m;
	OneHitDetector* filter;
	
	//this->midiDevice->setVolume(MIDI_CHANNEL, 126);



/**************** PRODUCTION PANTS ******************/


/**************** BETA PANTS ******************/
#if PRESET >= PRESET_BETA

#include "ConstantsSensors.h"

	// fix for signal bleed: set C and D ports to much higher cutoff if we can't detect them with jack switches
	#if ENABLE_JACK_SWITCHES
		#define PIEZO_C_D_CUTOFFRANGE_LOW PIEZO_CUTOFFRANGE_LOW
	#else
		#if PRESET >= PRESET_PREPRODUCTION
			#define PIEZO_C_D_CUTOFFRANGE_LOW 0.45
		#else
			#define PIEZO_C_D_CUTOFFRANGE_LOW 0.2
		#endif
	#endif


	////////////////////////////
	// OUTPUT FOR arduino 0
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;


#if PRESET == PRESET_BETA_NEIL_PEART || PRESET >= PRESET_PREPRODUCTION
	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);

#endif	


	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 1
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 2
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_C_D_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 3
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_C_D_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 4
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
#if PRESET == PRESET_BETA_NEIL_PEART
	// handle larger peizo type
	s->inRange.high = 0.56353861;
#else
	s->inRange.high = PIEZO_INRANGE_HIGH;
#endif
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;

#if PRESET == PRESET_BETA_NEIL_PEART	
	s->cutoffRange.low = 0.2;
#else
	s->cutoffRange.low = PIEZO_CUTOFFRANGE_LOW;
#endif	
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 5
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////


	////////////////////////////
	// OUTPUT FOR arduino 6
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_C_D_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

#if PRESET == PRESET_BETA_RYAN || PRESET == PRESET_BETA_NEIL_PEART || PRESET >= PRESET_PREPRODUCTION
	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
#endif	
	sensorInputs[i++] = s;
	////////////////////////////


// start NEIL PEART
#if PRESET == PRESET_BETA_NEIL_PEART
	////////////////////////////
	// OUTPUT FOR arduino 7
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
#if PRESET == PRESET_BETA_RYAN	
	// handle larger peizo type
	s->inRange.high = 0.56353861;
#else
	s->inRange.high = 0.16353861;
#endif
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.1;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////


	////////////////////////////
	// OUTPUT FOR arduino 8
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
#if PRESET == PRESET_BETA_RYAN	
	// handle larger peizo type
	s->inRange.high = 0.56353861;
#else
	s->inRange.high = 0.16353861;
#endif
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.1;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

// THIS SENSOR BE FUCKED UP

	// filter = new OneHitDetector();
	// filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	// s->addOutputFilter(filter);

	// m = new MidiMapping(this->midiDevice);
	// m->channel = MIDI_CHANNEL;
	// m->note = 60 + i;
	// m->setMsgType(MidiMapping::NOTE);
	// s->addMidiMapping(m);

	sensorInputs[i++] = s;
	////////////////////////////


// end NEIL PEART

#elif PRESET >= PRESET_PREPRODUCTION

	////////////////////////////
	// OUTPUT FOR arduino 7
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_C_D_CUTOFFRANGE_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////


#endif
// END WERID CRAP: TODO: REDO THIS


#if PRESET == PRESET_BETA || PRESET == PRESET_BETA_STRETCHY || PRESET == PRESET_BETA_RYAN || PRESET >= PRESET_PREPRODUCTION

#define FOOT_PEDAL_CC_ENABLE 0
#define FOOT_PEDAL_CC_SEND_NULLS 0


	////////////////////////////
	// OUTPUT FOR arduino 8
	////////////////////////////
	/// FOOT PEDAL
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = FOOT_PEDAL_INRANGE_HIGH;  
	s->outRange.low = FOOT_PEDAL_OUTRANGE_LOW;
	s->outRange.high = FOOT_PEDAL_OUTRANGE_HIGH;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
#if FOOT_PEDAL_CC_SEND_NULLS || !FOOT_PEDAL_CC_ENABLE
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); 
#else
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_HARD);
#endif	
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;


#if FOOT_PEDAL_CC_ENABLE
	#if FOOT_PEDAL_CC_SEND_NULLS	
	NullableOutputFilter* nullFilter = new NullableOutputFilter();
	s->addOutputFilter(nullFilter);
	#endif 

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 1; // modulation wheel
	m->setMsgType(MidiMapping::CONTROL_CHANGE);
	s->addMidiMapping(m);
#else
	
	// this one connection is weird on some of the boards: gets triggered by other piezos
	//#if PRESET < PRESET_PREPRODUCTION	
	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
	filter->sensitivityThreshold = DEFAULT_FOOT_PEDAL_SENSITIVITY_THRESHOLD;
	filter->highPassFilterConstant = FOOT_PEDAL_HIGHPASS_FILTER_CONST;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	//#endif

#endif	
	sensorInputs[i++] = s;
	////////////////////////////


	////////////////////////////
	// OUTPUT FOR arduino 9
	////////////////////////////
	/// FOOT PEDAL
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = FOOT_PEDAL_INRANGE_HIGH;  
	s->outRange.low = FOOT_PEDAL_OUTRANGE_LOW;
	s->outRange.high = FOOT_PEDAL_OUTRANGE_HIGH;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;


	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
	filter->highPassFilterConstant = FOOT_PEDAL_HIGHPASS_FILTER_CONST;
	filter->sensitivityThreshold = DEFAULT_FOOT_PEDAL_SENSITIVITY_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	#if PRESET >= PRESET_PREPRODUCTION	

	////////////////////////////
	// OUTPUT FOR arduino 10
	////////////////////////////
	/// FOOT PEDAL
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = FOOT_PEDAL_INRANGE_HIGH;  
	s->outRange.low = FOOT_PEDAL_OUTRANGE_LOW;
	s->outRange.high = FOOT_PEDAL_OUTRANGE_HIGH;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;


	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
	filter->highPassFilterConstant = FOOT_PEDAL_HIGHPASS_FILTER_CONST;
	filter->sensitivityThreshold = DEFAULT_FOOT_PEDAL_SENSITIVITY_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////


	////////////////////////////
	// OUTPUT FOR arduino 11
	////////////////////////////
	/// FOOT PEDAL
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = FOOT_PEDAL_INRANGE_HIGH;  
	s->outRange.low = FOOT_PEDAL_OUTRANGE_LOW;
	s->outRange.high = FOOT_PEDAL_OUTRANGE_HIGH;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;


	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
	filter->highPassFilterConstant = FOOT_PEDAL_HIGHPASS_FILTER_CONST;
	filter->sensitivityThreshold = DEFAULT_FOOT_PEDAL_SENSITIVITY_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////


	#endif
#endif








































/**************** ARDUINO PANTS ******************/


#elif PRESET == PRESET_JEANS

	////////////////////////////
	// OUTPUT FOR arduino 0
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 1;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.8428571;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;
	s->isLogarithmic = false; // since this is a ribbon sensor.

	// s->addOutputFilter(new OneHitDetector());

	// m = new MidiMapping(this->midiDevice);
	// m->channel = 0;
	// m->note = 60 + i;
	// m->setMsgType(MidiMapping::NOTE);
	// s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 1
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.21212122;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.094285714;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 2
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.16353861;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.01;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 3
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.28739002;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.09;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 4
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.20821114;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.01;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 5
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.23851417;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.54285717;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = 170;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL;
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



#elif PRESET == PRESET_NEON_GREEN_STRING


	////////////////////////////
	// OUTPUT FOR arduino 0
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.3998045;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 1
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.1544477;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 2
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.0684262;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 3
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 1;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 4
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0.24340177;
	s->inRange.high = 1;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;


	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	filter->highPassFilterConstant = FOOT_PEDAL_HIGHPASS_FILTER_CONST;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 5
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 1;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	// filter = new OneHitDetector();
	// filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	// s->addOutputFilter(filter);

	// m = new MidiMapping(this->midiDevice);
	// m->channel = MIDI_CHANNEL; //0 for durms!
	// m->note = 60 + i;
	// m->setMsgType(MidiMapping::NOTE);
	// s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////


#elif PRESET == PRESET_VJACKET_LEATHER

	////////////////////////////
	// OUTPUT FOR arduino 0
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 1;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.8428571;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;
	s->isLogarithmic = false; // since this is a ribbon sensor.

	// s->addOutputFilter(new OneHitDetector());

	// m = new MidiMapping(this->midiDevice);
	// m->channel = 0;
	// m->note = 60 + i;
	// m->setMsgType(MidiMapping::NOTE);
	// s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 1
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.40714276;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.22857143;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 2
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.6642857;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.021428572;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 3
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.62142855;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.021428572;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 4
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 1;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.028571429;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

	m = new MidiMapping(this->midiDevice);
	m->channel = MIDI_CHANNEL; //0 for durms!
	m->note = 60 + i;
	m->setMsgType(MidiMapping::NOTE);
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////



	////////////////////////////
	// OUTPUT FOR arduino 5
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = 0.23851417;
	s->outRange.low = PIEZO_OUTRANGE_LOW;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.54285717;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE); //Nullable
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = false;
	s->isLogarithmic = false; // since this is a light sensor.

	// filter = new OneHitDetector();
	// filter->retriggerThreshold = 170;
	// s->addOutputFilter(filter);

	// m = new MidiMapping(this->midiDevice);
	// m->channel = MIDI_CHANNEL;
	// m->note = 60 + i;
	// m->setMsgType(MidiMapping::NOTE);
	// s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	////////////////////////////











#else







	//arduino 0
s = new SensorOutput();
s->inRange.low = 0.028571429;
s->inRange.high = 0.30;//35;
s->outRange.low = PIEZO_OUTRANGE_LOW;
s->outRange.high = 1;
s->cutoffRange.low = 0.014285714;
s->cutoffRange.high = 0.95;
s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

// m = new MidiMapping(this->midiDevice);
// m->channel = MIDI_CHANNEL;
// m->note = 60 + i;
// s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 1
s = new SensorOutput();
s->inRange.low = 0.021428572;
s->inRange.high = 0.40;//44285715;
s->outRange.low = PIEZO_OUTRANGE_LOW;
s->outRange.high = 1;
s->cutoffRange.low = 0.057142872;
s->cutoffRange.high = 1;
s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

m = new MidiMapping(this->midiDevice);
m->channel = MIDI_CHANNEL;
m->note = 57;//60 + i;
m->setMsgType(MidiMapping::NOTE);
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 2
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.40;//47142863;
s->outRange.low = PIEZO_OUTRANGE_LOW;
s->outRange.high = 1;
s->cutoffRange.low = 0.1;
s->cutoffRange.high = 1;
s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_RETRIGGER_THRESHOLD;
	s->addOutputFilter(filter);

m = new MidiMapping(this->midiDevice);
m->channel = MIDI_CHANNEL;
m->note = 60 + i;
m->setMsgType(MidiMapping::NOTE);
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 3
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.40;//5;
s->outRange.low = PIEZO_OUTRANGE_LOW;
s->outRange.high = 1;
s->cutoffRange.low = 0.1;
s->cutoffRange.high = 1;
s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

filter = new OneHitDetector();
	filter->retriggerThreshold = 50;
	s->addOutputFilter(filter);

m = new MidiMapping(this->midiDevice);
m->channel = MIDI_CHANNEL;
m->note = 60 + i;
m->setMsgType(MidiMapping::NOTE);
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 4
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.40;//5;
s->outRange.low = PIEZO_OUTRANGE_LOW;
s->outRange.high = 1;
s->cutoffRange.low = 0.1;
s->cutoffRange.high = 1;
s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

filter = new OneHitDetector();
	filter->retriggerThreshold = 50;
	s->addOutputFilter(filter);

m = new MidiMapping(this->midiDevice);
m->channel = MIDI_CHANNEL;
m->note = 60 + i;
m->setMsgType(MidiMapping::NOTE);
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 5
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.4;
s->outRange.low = PIEZO_OUTRANGE_LOW;
s->outRange.high = 1;
s->cutoffRange.low = 0.1;
s->cutoffRange.high = 1;
s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

filter = new OneHitDetector();
	filter->retriggerThreshold = 50;
	s->addOutputFilter(filter);

m = new MidiMapping(this->midiDevice);
m->channel = MIDI_CHANNEL;
m->note = 60 + i;
m->setMsgType(MidiMapping::NOTE);
s->addMidiMapping(m);

sensorInputs[i++] = s;

#endif

}


void SensorizerServer::loadNotes(byte notes[]) {
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL) {
			if (sensorInputs[i]->dropdownMidiMappings[0] != NULL)
				sensorInputs[i]->dropdownMidiMappings[0]->note = notes[min(i, (NOTE_PRESETS_ELEMENT_LENGTH - 1))];
		}
	}
}

// this is a very stupip way of doing this. this function should not exist
int SensorizerServer::getSensorIdxForNote(byte note) {
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL) {
			if (sensorInputs[i]->dropdownMidiMappings[0] != NULL) {
				if (note == sensorInputs[i]->dropdownMidiMappings[0]->note) {

/* We have to disable this fix for DRUM-794, since it messes up the Configurator. See DRUM-843 */
#if FALSE && PRESET >= PRESET_PREPRODUCTION
					// now we do something really silly. 
					// the first batch drumpads are wired incorrectly - the drumpads are swapped in position.
					// so we need to unswap them here, so the apps' graphics line up correctly with the physical product. (see DRUM-794)
					// whoa thats kind of weird... this is basically the junction between the real and virtual. 
					// the phantom joint upon which all this silicon and bits rely.


					if (i < 4) { 		// if col 0 (bottom sensor)
						return i + 4;	// move to col 1 (top sensor)
					}
					else if (i < 8) { 	// if col 1 (bottom sensor)
						return i - 4;	// move to col 0 (top sensor)
					}
					else { 			// if col 2 (foot pedal)
						return i;	// do not translate
					}
#else
					return i;
#endif					

 				}
			}
		}
		else {
			break;
		}
	}

	return -1;
}
