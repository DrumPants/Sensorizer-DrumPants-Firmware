
#include "LedInterface.h"

#define NUM_TICKS_FOR_TEMPORARY_DISPLAY 1000

void LEDInterface::setup() {
  led->setup(); 
  
  this->curBank = 0;
  this->numTicksLeftForTemporary = 0;

  led->setDigit(0, '0');
  led->setDigit(1, '0');
  led->setDecimalPoint(0, false);
  led->setDecimalPoint(1, false);
}

void LEDInterface::check() {
  led->tick();

  // once temporary has been shown long enough, go back to showing the bank.
  if (this->numTicksLeftForTemporary > 0) {

  	if (--this->numTicksLeftForTemporary == 0) {
  		this->changeBank(NULL, this->curBank);
  	}
  }
}

void LEDInterface::changeBank(const char* label, BankId bankId) {
  this->curBank = bankId;

  led->print(bankId);
  led->setDecimalPoint(0, false);
}

void LEDInterface::changeScale(ScaleId scaleId) {
  led->print(scaleId);
  led->setDecimalPoint(0, true);
}

void LEDInterface::showTemporarily(int num) {
  led->print(num);
  led->setDecimalPoint(0, false);
  led->setDecimalPoint(1, false);

  this->numTicksLeftForTemporary = NUM_TICKS_FOR_TEMPORARY_DISPLAY;
}
