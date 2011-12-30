 /* NEEDED??

 */
 
#include "SensorizerServer.h"


SensorizerServer::SensorizerServer() {
	this->midiDevice = new MidiDevice(); //Soft TX on 3, we don't use RX in this code
}

SensorizerServer::~SensorizerServer() {
	for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
		if (sensorInputs[i] != NULL)
			delete sensorInputs[i];
	}

	delete this->midiDevice;
}

//reads all input devices values into the sensorInput objects.
//also sends all values from sensor inputs.
void readAll() {
	//if (arduino != null) {
		//arduino bindings
		for (int i = 0; i < SENSOR_INPUTS_LENGTH; i++) {
	//		if (sensorInputs[i].deviceName == "arduino") {
				//float v = (float)arduino.analogRead(sensorInputs[i].getDeviceInputIdx()) / 1023.0f;
				float v = arduino.read(sensorInputs[i].getDeviceInputIdx());
				
				//if (v > 0)
				//	Debug.print(Debug.DEBUG, "ardunio input " + i + ": " + v);		
				
				sensorInputs[i]->setValue(v);
				sensorInputs[i]->send();
	//		}
		}
	//}
}

void SensorizerServer::loadPreset() {
	int i = 0;
	SensorOutput* s = new SensorOutput();
	s->inRange.low = 0.028571429;
	s->inRange.high = 0.35;
	s->outRange.low = 0;
	s->outRange.high = 1;
	s->cutoffRange.low = 0.014285714;
	s->cutoffRange.high = 0.95;
	s->setCutoffType(SensorOutput::CUTOFF_TYPE_VAL_NULLABLE);
	
	MidiMapping* m = new MidiMapping(this->midiDevice);
	m->channel = 1;
	m->note = 60 + i;
	s->addMidiMapping(m);
	
	sensorInputs[i++] = s;
	
}

