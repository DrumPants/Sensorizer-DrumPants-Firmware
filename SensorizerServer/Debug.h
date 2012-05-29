#ifndef SensorizerDebug_h
#define SensorizerDebug_h

#define ENABLE_DEBUG_PRINTING 0

#if ENABLE_DEBUG_PRINTING
	#define DEBUG_PRINT(text) Serial.println(text);Serial.println("-----");
	#define DEBUG_PRINT_NUM(text, num) Serial.print(text);Serial.println(num, DEC);Serial.println("-----");
#else
	#define DEBUG_PRINT(text)   
	#define DEBUG_PRINT_NUM(text, num)   
#endif

#endif