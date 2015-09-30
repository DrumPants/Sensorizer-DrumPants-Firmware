Sensorizer for Arduino + DrumPants Firmware
================================================

Sensorizer is an Arduino library for converting analog sensor readings into musical MIDI notes. It is the basis of the firmware for the DrumPants wearable musical instrument, and is designed for creating your own Arduino musical instrument or MIDI drum brain.

http://www.drumpants.com

Currently it is supported on Arduino Due and the DrumPants hardware, although with some work could be made to work on AVR Arduino as well.

Features
--------------------------

* Sends MIDI messages over native USB, 
* As well as the other UARTs to connect to Bluetooth and/or MIDI synthesizers
* Sensitivity Adjustments for piezo drum pads, and other resistive sensors like pressure and photoresisitors. 
* Algorithms for detecting drum hits and filtering false triggers and noise
* Customizable presets system
* Configurator for altering presets via MIDI sysex and saving to EEPROM
* I2C EEPROM Storage Driver
* Scales and Sound Bank selection
* 7-segment LED Display Driver (hexanumeric)
* Encoder knob and volume buttons UI Driver
* Hardware Test Suite
* Blood, sweat, and beats


Installation
==========================

You can use Sensorizer like any other Arduino library sketch directly from the Arduino IDE.

Tested on Arduino IDE 1.6.2. 


Install Library Dependencies
--------------------------

Install each of these in your `Arduino/libraries` folder:

1. `Sensorizer/SensorizerServer` folder
2. `Sensorizer/LEDController/SevenSegmentController` folder (only copy the SevenSegmentController folder)
3. Rob Tillaart's I2C_EEPROM libary: https://github.com/odbol/Arduino-1.git (just the I2C_EEPROM folder)
4. Paul Stoffregen's Encoder library: http://www.pjrc.com/teensy/td_libs_Encoder.html
5. *IMPORTANT*: Copy all files from the `SensorizerServer/**` subfolders into a single folder called `SensorizerServer/utility`. (This is to circumvent the Arduino compile process that ignores source files in any subfolders in a library). For convenience, you can run the `SensorizerServer/fix_utility_dir_for_arduino.sh` script to do it for you.


Compile Sketch and Upload
-------------------------

1. Plug the USB into your DrumPants unit, or into an Arduino Due's Native USB Port.
2. Open the `Sensorizer.ino` file in the Arduino IDE.
3. Choose `Tools->Board->Arduino Due` (Native USB Port)
4. Choose the correct serial port.
5. Press the `Upload` button.
6. MIDI data should be sent over the USB Serial interface when you hit a DrumPad.


If something goes wrong, or you want to revert to the DrumPants' factory firmware:

1. Ensure the DrumPants are turned on.
2. Reset and Erase the DrumPants PCB by shorting together the Reset pins and quickly afterward the Erase pins. The Reset and Erase pins are on the back of the PCB, near the USB port.
3. Repeat step 2 several times to make sure it's erased.
4. Use the DrumPants desktop app to upload the DrumPants firmware: http://apps.drumpants.com


Arduino Due Considerations
--------------------------

If you are using your own sensors on an Arduino Due, keep in mind the DrumPants
hardware includes a switch on the sensor jacks that disables sensor readings on any pin 
who's jack is not connected. See `SensorJackSwitches.cpp` for details, or just set ENABLE_JACK_SWITCHES to 0.	


Bug Reporting / Contributing
==========================

Please file a Github issue for any questions or bugs. 

Pull requests welcome! Please associate with a related Issue if possible.


Roadmap
--------------------------

Upcoming features in DrumPants 2.0:

* Class-compliant USB MIDI device
* Customizable preset banks
* Foot pedal control (i.e. switching presets on-the-fly based on sensor output)



License
==========================

Copyright 2011-2015 DrumPants, Inc.
  
http://www.drumpants.com

Please see LICENSE.txt for copying and licensing info.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
