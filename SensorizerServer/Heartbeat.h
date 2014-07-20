#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "Constants.h"
#include <Arduino.h>
#include "Metro.h"

class Heartbeat {
	
public:
	Heartbeat();

	void tick();

private:

	Metro metro;

	void writeToSerial(byte data);
};


#endif