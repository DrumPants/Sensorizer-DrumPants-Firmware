/****

Controls a seven-segment LED connected to a darlington array.

***/

#ifndef _SevenSegmentController_h
#define _SevenSegmentController_h

#include <Arduino.h>
//typedef unsigned char byte;
//#include <Constants.h>
//#include <SensorizerServer.h>


#define NUM_DIGITS 2

#define BLANK_DIGIT 'G'

/**** PINOUTS ***/
// this is because arduino doesn't keep its defines in context for the rest of the compiile. WTF!
#ifndef IS_DRUMPANTS 
#define IS_DRUMPANTS 1
#endif

#if IS_DRUMPANTS
  const unsigned char outs[] = {34, 35, 36, 37, 38, 39, 40};
  
  // this is for the LED-LDD-E304NI-RA-Lumex right-angle LED, 
  // which has different pinouts for A-G depending on which digit you're activating. NICE
  #define IS_RETARDED_LED 1
  #if IS_RETARDED_LED
  const unsigned char outsDigit1[] = {34, 38, 35, 37, 39, 36, 40};
  #endif
  
  #define PIN_DIGIT_0 44
  #define PIN_DIGIT_1 45  
#elif IS_DUE
  const unsigned char outs[] = {41, 37, 40, 38, 35, 39, 36};
  #define PIN_DIGIT_0 42
  #define PIN_DIGIT_1 43
#else
  const unsigned char outs[] = {5, 6, 7, 8, 9, 10, 11};
  #define PIN_DIGIT_0 12
  #define PIN_DIGIT_1 13

#endif


class SevenSegmentController {

    long long lastUpdate;

	/***
		Chooses which digit we are displaying during multiplexing (which one gets power).
	***/
	void chooseDigit(byte digit);


	void showChar(unsigned char c);
	void showDigit(byte num);

public: 
	void setup();
	void tick();

	/***
		Sets the given digit to the given character '0'-'9', 'A'-'F', or BLANK_DIGIT (and maybe others one day).
	***/
	void setDigit(byte digitIndex, byte character);

	/***
		Prints the given number as its decimal equivalent.
	***/
	void print(int num);
};



#endif