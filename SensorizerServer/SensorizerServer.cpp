 /* NEEDED??

 */
 
#include "SensorizerServer.h"


SensorizerServer::SensorizerServer() {
	//manually init array
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		sensorInputs[i] = NULL;
	}

	this->midiDevice = new MidiDevice(); 

	this->loadPreset();
}

SensorizerServer::~SensorizerServer() {
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL)
			delete sensorInputs[i];
	}

	delete this->midiDevice;
}


void SensorizerServer::init() {
	this->midiDevice->setup();
	
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL)
			sensorInputs[i]->init();
	}

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
	float v = pinValue / 1024.0;

DEBUG_PRINT_NUM("readPin", v)
  
	sensorInputs[pinIdx]->setValue(v);
	sensorInputs[pinIdx]->send();
}

void SensorizerServer::loadPreset() {
	int i = 0;
	SensorOutput* s;
	MidiMapping* m;
	
	//arduino 0
s = new SensorOutput();
s->inRange.low = 0.028571429;
s->inRange.high = 0.35;
s->outRange.low = 0;
s->outRange.high = 1;
s->cutoffRange.low = 0.014285714;
s->cutoffRange.high = 0.95;
s->setCutoffType(3); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

// m = new MidiMapping(this->midiDevice);
// m->channel = 1;
// m->note = 60 + i;
// s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 1
s = new SensorOutput();
s->inRange.low = 0.03;
s->inRange.high = 0.16428576;
s->outRange.low = 0;
s->outRange.high = 1;
s->cutoffRange.low = 0.035714287;
s->cutoffRange.high = 0.98571426;
s->setCutoffType(3); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

m = new MidiMapping(this->midiDevice);
m->channel = 1;
m->note = 57;//60 + i;
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 2
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.10714287;
s->outRange.low = 0;
s->outRange.high = 1;
s->cutoffRange.low = 0.057142872;
s->cutoffRange.high = 0.98571426;
s->setCutoffType(3); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

m = new MidiMapping(this->midiDevice);
m->channel = 1;
m->note = 60 + i;
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 3
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.100000024;
s->outRange.low = 0;
s->outRange.high = 1;
s->cutoffRange.low = 0.05;
s->cutoffRange.high = 0.98571426;
s->setCutoffType(3); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

m = new MidiMapping(this->midiDevice);
m->channel = 1;
m->note = 60 + i;
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 4
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.23571432;
s->outRange.low = 0;
s->outRange.high = 1;
s->cutoffRange.low = 0.035714287;
s->cutoffRange.high = 0.98571426;
s->setCutoffType(3); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

m = new MidiMapping(this->midiDevice);
m->channel = 1;
m->note = 60 + i;
s->addMidiMapping(m);

sensorInputs[i++] = s;


//arduino 5
s = new SensorOutput();
s->inRange.low = 0;
s->inRange.high = 0.4928572;
s->outRange.low = 0;
s->outRange.high = 1;
s->cutoffRange.low = 0.014285714;
s->cutoffRange.high = 1;
s->setCutoffType(3); //Nullable
s->multiplyVal = 1;
s->addVal = 0;
s->isInvert = false;

//m = new MidiMapping(this->midiDevice);
//m->channel = 1;
//m->note = 60 + i;
//s->addMidiMapping(m);

sensorInputs[i++] = s;
}

