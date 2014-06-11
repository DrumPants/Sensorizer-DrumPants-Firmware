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
	int curTime = metro.hasTicked();

	if (curTime == 0)
		return;

#if ENABLE_LOOPER
	this->looper->tick(curTime);
#endif
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
	
	this->midiDevice->setVolume(MIDI_CHANNEL, 126);



/**************** PRODUCTION PANTS ******************/


/**************** BETA PANTS ******************/
#if PRESET >= PRESET_BETA

	#define DEFAULT_FOOT_PEDAL_SENSITIVITY_THRESHOLD 0.3
	#define FOOT_PEDAL_CUTOFF_LOW 0.35

	#if PRESET >= PRESET_PREPRODUCTION	
		// handle sensitive-ass bare peizos
		#define PIEZO_INRANGE_HIGH 1.0;
		#define PIEZO_CUTOFFRANGE_LOW 0.38

		#define DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD DEFAULT_RETRIGGER_THRESHOLD
	#elif PRESET == PRESET_BETA_RYAN
		// handle larger peizo type
		#define PIEZO_INRANGE_HIGH 0.56353861	
		#define PIEZO_CUTOFFRANGE_LOW 0.1

		#define DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD (DEFAULT_RETRIGGER_THRESHOLD * 6)
	#else
		#define PIEZO_INRANGE_HIGH 0.16353861
		#define PIEZO_CUTOFFRANGE_LOW 0.1

		#define DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD (DEFAULT_RETRIGGER_THRESHOLD * 6)
	#endif


	////////////////////////////
	// OUTPUT FOR arduino 0
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	// OUTPUT FOR arduino 3
	////////////////////////////
	s = new SensorOutput();
	s->inRange.low = 0;
	s->inRange.high = PIEZO_INRANGE_HIGH;
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
	s->outRange.high = 1;
	s->cutoffRange.low = PIEZO_CUTOFFRANGE_LOW;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->inRange.high = 1;
	s->outRange.low = 0;
	s->outRange.high = 1;
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
	s->inRange.high = 1;
	s->outRange.low = 0;
	s->outRange.high = 1;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
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
	s->inRange.high = 1.0;
	s->outRange.low = 0;
	s->outRange.high = 1;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
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
	s->inRange.high = 1.0;
	s->outRange.low = 0;
	s->outRange.high = 1;
	s->cutoffRange.low = FOOT_PEDAL_CUTOFF_LOW;
	s->cutoffRange.high = 1;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE_LOW); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;

	filter = new OneHitDetector();
	filter->retriggerThreshold = DEFAULT_FOOT_PEDAL_RETRIGGER_THRESHOLD;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
	s->outRange.high = 1;
	s->cutoffRange.low = 0;
	s->cutoffRange.high = 1;
	s->setCutoffType(3); //No Cutoff
	s->multiplyVal = 1;
	s->addVal = 0;
	s->isInvert = true;

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
	s->inRange.high = 1;
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
	s->outRange.low = 0;
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
s->outRange.low = 0;
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
s->outRange.low = 0;
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
s->outRange.low = 0;
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
s->outRange.low = 0;
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
s->outRange.low = 0;
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
s->outRange.low = 0;
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

int SensorizerServer::getSensorIdxForNote(byte note) {
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL) {
			if (sensorInputs[i]->dropdownMidiMappings[0] != NULL) {
				if (note == sensorInputs[i]->dropdownMidiMappings[0]->note) {
					return i;
 				}
			}
		}
		else {
			break;
		}
	}

	return -1;
}
