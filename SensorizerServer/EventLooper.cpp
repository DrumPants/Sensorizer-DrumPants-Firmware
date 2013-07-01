/***
	EVENT LOOPER

	Listens for sensor inputs and records them to be looped.

***/


void EventLooper::setOutputDevice(MidiDevice* d) {
	this->device = d;
}

void EventLooper::send(MidiEvent* ev) {
	this->device->talkMIDI(ev->cmd, ev->data1, ev->data2);
}

/***
	The listener function. Called by a sensor input whenever it sends an output.

***/
void EventLooper::onSendOutput(byte cmd, byte data1, byte data2) {
	LoopLayer curLayer = this->layers[this->curLayerIdx];

	curLayer.addEvent(MIDIEventPool.createEvent(this->curTick, cmd, data1, data2));
}

void EventLooper::tick(int curTime) {

	this->curTick = (this->curTick + 1) % LOOP_LENGTH_IN_TICKS;

	for(int i = 0; i < this->layers.length; i++) {
		LoopLayer l = this->layers[i];

		MidiEvent ev = l.events[l.curEventIdx];

		while (ev.tickTime <= curTick) {
			this->send(ev);

			l.curEventIdx++;
			ev = l.events[l.curEventIdx];
		}
	}

	this->lastTickTime = curTime;
}