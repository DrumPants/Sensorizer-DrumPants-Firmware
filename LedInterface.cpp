
#include "LedInterface.h"

void LEDInterface::setup() {
  led->setup(); 
  
  led->setDigit(0, '0');
  led->setDigit(1, '0');
}

void LEDInterface::check() {
  led->tick();
}

void LEDInterface::changeBank(const char* label, BankId bankId) {
  led->print(bankId);
}

void LEDInterface::changeScale(ScaleId scaleId) {
  led->print(scaleId);
}
