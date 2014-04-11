#include "SevenSegmentController.h"

#define NUM_OUTS 7
#define NUM_PRINTABLE_DIGITS 17


// Define the LED digit patters, from 0 - 9
// Note that these patterns are for common cathode displays
//                                    Arduino pin: 2,3,4,5,6,7,8
const byte seven_seg_digits[NUM_PRINTABLE_DIGITS][NUM_OUTS] = { 
                                      { 1,1,1,1,1,1,0 },  // = 0
                                      { 0,1,1,0,0,0,0 },  // = 1
                                      { 1,1,0,1,1,0,1 },  // = 2
                                      { 1,1,1,1,0,0,1 },  // = 3
                                      { 0,1,1,0,0,1,1 },  // = 4
                                      { 1,0,1,1,0,1,1 },  // = 5
                                      { 1,0,1,1,1,1,1 },  // = 6
                                      { 1,1,1,0,0,0,0 },  // = 7
                                      { 1,1,1,1,1,1,1 },  // = 8
                                      { 1,1,1,0,0,1,1 },   // = 9
                                                                            
                                      // // EXTENDOTRON
                                      { 1,1,1,0,1,1,1 },  // = A
                                      { 0,0,1,1,1,1,1 },  // = B
                                      { 1,0,0,1,1,1,0 },  // = C
                                      { 0,1,1,1,1,0,1 },  // = D
                                      { 1,0,0,1,1,1,1 },  // = E
                                      { 1,0,0,0,1,1,1 },  // = F
                                      { 0,0,0,0,0,0,0 }   // = BLANK_DIGIT
                                      };
             

#define LED_UPDATE_INTERVAL 1
  byte digitToShow = 0;


  byte charactersForDigits[NUM_DIGITS] = {'3', '3'};

#ifdef PIN_DECIMAL_POINT   
  bool decimalPoints[NUM_DIGITS] = {false, false};                                           
#endif

void SevenSegmentController::setup() {
  this->lastUpdate = 0;
  // put your setup code here, to run once:

  for (int i = 0; i < NUM_OUTS; i++) {
    pinMode(outs[i], OUTPUT);   
  }
  
   pinMode(PIN_DIGIT_0, OUTPUT);  
   pinMode(PIN_DIGIT_1, OUTPUT);  

#ifdef PIN_DECIMAL_POINT   
   pinMode(PIN_DECIMAL_POINT, OUTPUT);   
#endif
}


void SevenSegmentController::tick() {
  
  //showDigit(digit);
  long long curMillis = millis();
  if (curMillis - this->lastUpdate > LED_UPDATE_INTERVAL) {

    

    digitToShow = (digitToShow + 1) % NUM_DIGITS;
    chooseDigit(digitToShow);
    //showDigit(digitToShow ? digit : digit);
    showChar(charactersForDigits[digitToShow]);

#ifdef PIN_DECIMAL_POINT   
    digitalWrite(PIN_DECIMAL_POINT, decimalPoints[digitToShow] ? HIGH : LOW);    
#endif    

    this->lastUpdate = curMillis;
  }
}

void SevenSegmentController::chooseDigit(byte digit) {

     // SerialUSB.print("chooseDigit "); 
     // SerialUSB.println(digit);

  digitalWrite(PIN_DIGIT_0, digit ? LOW : HIGH);
  digitalWrite(PIN_DIGIT_1, digit ? HIGH : LOW);
}


void SevenSegmentController::showChar(unsigned char c) {
  byte num;

  if (c >= 'A') {
    num = c - 'A' + 10; // start after normal digits in array
  }
  else {
    // normal digit
    num = c - '0';  
  }
  

//      SerialUSB.print("show char "); 
// SerialUSB.print(NUM_OUTS);
//      SerialUSB.println(num);

  showDigit(num);
}
  
void SevenSegmentController::showDigit(byte num) {
  if (num >= NUM_PRINTABLE_DIGITS) return;

  const byte* segs = seven_seg_digits[num];
  
  for (int i = 0; i < NUM_OUTS; i++) {
    byte val = segs[i];
   
    unsigned char segPin;
#if IS_RETARDED_LED
    segPin = (digitToShow == 0 ? outs[i] : outsDigit1[i]);
#else
    segPin = outs[i];
#endif

    digitalWrite(segPin, val == 0 ? LOW: HIGH); // common ANODE   
    
  //SerialUSB.print(outs[i]); 
  // SerialUSB.print(": ");
  // SerialUSB.println(i); 
  }
}



void SevenSegmentController::setDigit(byte digitIndex, byte character) {
  charactersForDigits[digitIndex] = character;
}


void SevenSegmentController::print(int num) {
  // byte tens = num / 10;

  // charactersForDigits[0] = tens + '0';
  // charactersForDigits[1] = (num % 10) + '0';
  // 
  
  String str = String(num);

  // only show the two least significant digits
  int startChar = str.length() - 2;

  // handle single digits
  if (startChar < 0) {
      charactersForDigits[0] = '0';
  }
  else if (startChar > 0) {
    // translate hundreds into hex-ish notation.
    byte tens = num / 10;

    charactersForDigits[0] = ('A' - 10) + tens;
  }
  else {
    charactersForDigits[0] = str.charAt(startChar + 0);
  }

  charactersForDigits[1] = str.charAt(startChar + 1);
}



void SevenSegmentController::setDecimalPoint(int decimalIdx, bool isOn) {

#ifdef PIN_DECIMAL_POINT   
  decimalPoints[decimalIdx] = isOn;
#endif
  
}