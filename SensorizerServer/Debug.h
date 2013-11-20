#ifndef SensorizerDebug_h
#define SensorizerDebug_h

#define ENABLE_DEBUG_PRINTING 1

#if IS_DRUMPANTS
	#define SerialToComputer SerialUSB
#else
	#define SerialToComputer Serial
#endif

#define DEBUG_PRINT_SETUP SerialToComputer.begin(57600);SerialToComputer.println("Debug printing STARTED");

#define PRINT(text) SerialToComputer.println(text);SerialToComputer.println("-----");
#define PRINT_NUM(text, num) SerialToComputer.print(text);SerialToComputer.println(num, DEC);SerialToComputer.println("-----");
#define PRINT_NUMS(text, num, num2) SerialToComputer.print(text);SerialToComputer.print(num, DEC);SerialToComputer.print(", ");SerialToComputer.println(num2, DEC);SerialToComputer.println("-----");
 
#if ENABLE_DEBUG_PRINTING
	#define DEBUG_PRINT(text) PRINT(text)
	#define DEBUG_PRINT_NUM(text, num) PRINT_NUM(text, num)
#else
	#define DEBUG_PRINT(text)   
	#define DEBUG_PRINT_NUM(text, num)   
#endif

#endif