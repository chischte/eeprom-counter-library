//TODO:
// implement size of enum
//enum counter{longTimeCounter,shortTimeCounter,toolIdentNumber,endOfEnum};

#include <EEPROM_Counter.h>

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Counter exampleCounter(1024,3);
// The values in the brackets define the eeprom size and the number of values to be stored.
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P)
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560)





void setup() {

  Serial.begin(9600);
}

void loop() {
}
