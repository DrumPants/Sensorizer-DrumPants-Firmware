#ifndef SensorizerDebug_h
#define SensorizerDebug_h

#define ENABLE_DEBUG_PRINTING 0

#define PRINT(text) Serial.println(text);Serial.println("-----");
#define PRINT_NUM(text, num) Serial.print(text);Serial.println(num, DEC);Serial.println("-----");
#define PRINT_NUMS(text, num, num2) Serial.print(text);Serial.print(num, DEC);Serial.print(", ");Serial.println(num2, DEC);Serial.println("-----");
 
#if ENABLE_DEBUG_PRINTING
	#define DEBUG_PRINT(text) PRINT(text)
	#define DEBUG_PRINT_NUM(text, num) PRINT_NUM(text, num)
#else
	#define DEBUG_PRINT(text)   
	#define DEBUG_PRINT_NUM(text, num)   
#endif

#endif