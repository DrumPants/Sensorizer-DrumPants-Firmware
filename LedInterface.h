#include <SevenSegmentController.h>

/**
 * Handles the knobs and buttons for the menu system.
 * 
 */

#ifndef LEDInterface_h_
#define LEDInterface_h_

#include <Arduino.h>
#include <Constants.h>
#include <SensorizerServer.h>

#include <SevenSegmentController.h>


// these are indexes of the arrays holding scales. they should actually be enums or something.
typedef int ScaleId;
typedef int BankId;
 
struct LEDInterface {

	// holds the current bank for temporary printing to something else.
	BankId curBank;

	int numTicksLeftForTemporary;

	SevenSegmentController* led;

public: 

	LEDInterface() {
		led = new SevenSegmentController();
	};

	~LEDInterface() {
		delete led;
	};

	void setup();
	void check();

	void changeBank(const char* label, BankId bankId);
	void changeScale(ScaleId scaleId);


	void showTemporarily(int num);
};



 #endif
