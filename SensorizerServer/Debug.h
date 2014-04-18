#ifndef SensorizerDebug_h
#define SensorizerDebug_h

#define ENABLE_DEBUG_PRINTING 1

// if 1, we'll relay all debug messages from the BLE module's HCI UART to the SerialToComputer
#define ENABLE_DEBUG_PRINTING_RELAY_FROM_BLE 1

#if IS_DUE
	// SerialUSB is BROKEN
	#define SerialToComputer SerialUSB
#else
	#define SerialToComputer Serial
#endif

// don't use this function! it fucks things up for some reason!
#define DEBUG_PRINT_SETUP SerialToComputer.begin(57600);SerialToComputer.println("Debug printing STARTED");

#define PRINT(text) SerialToComputer.println(text);SerialToComputer.println("-----");
#define PRINT_NUM(text, num) SerialToComputer.print(text);SerialToComputer.println(num, DEC);SerialToComputer.println("-----");
#define PRINT_NUMS(text, num, num2) SerialToComputer.print(text);SerialToComputer.print(num, DEC);SerialToComputer.print(", ");SerialToComputer.println(num2, DEC);SerialToComputer.println("-----");
 
#if ENABLE_DEBUG_PRINTING
	#define DEBUG_PRINT(text) PRINT(text)
	#define DEBUG_PRINT_NUM(text, num) PRINT_NUM(text, num)
	#define DEBUG_PRINT_NUMS(text, num, num2) PRINT_NUMS(text, num, num2)
#else
	#define DEBUG_PRINT(text)   
	#define DEBUG_PRINT_NUM(text, num)   
	#define DEBUG_PRINT_NUMS(text, num, num2)
#endif

#endif