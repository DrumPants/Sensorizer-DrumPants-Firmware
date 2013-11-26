
#define IS_DRUMPANTS 1
#define IS_DUE 1

#include <Arduino.h>
#include <SevenSegmentController.h>

#define TEST_PRINT 1
#define TEST_SET 0
#define TEST TEST_PRINT

SevenSegmentController led;

void setup() {
  led.setup();
   
   SerialUSB.begin(57600);
}

long interval = 1000;
long curCount = 0;

long long lastUpdate = 0;

const int updateInterval = 20;


void loop() {
  

  long long curMillis = millis();
  if (curMillis - lastUpdate > updateInterval) {
    ++curCount;
    int digit = ((double)curCount / (double)interval) * 10.0;
    
    if (curCount > interval) {
      curCount = 0;
     SerialUSB.println(curCount); 
     SerialUSB.println(digit); 
    }
    
     SerialUSB.println(digit); 
  //SerialUSB.println(digit);
#if TEST == TEST_PRINT    
    led.print(curCount / 10);
#elif TEST == TEST_SET
    led.setDigit(0, '4');
    led.setDigit(1, '2');
#endif    
    
    lastUpdate = curMillis;
  }

  led.tick();
}
