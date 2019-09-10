
#include <EEPROM_Counter.h>

// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
// the list can have between 1 and 255 value entries.
enum counter {
  longTimeCounter,  // example value name
  shortTimeCounter, // example value name
  toolIdentNumber,  // example value name
  endOfEnum //dont touch this one
};
const byte numberOfValues = endOfEnum;

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P)
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560)
const unsigned int eepromSize = 1024;

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Counter exampleCounter(eepromSize, numberOfValues);
// The values in the brackets define the eeprom size and the number of values to be stored.


void setup() {
  Serial.begin(9600);
}

void loop() {

  //exampleCounter.setAllZero();


  exampleCounter.countOneUp(longTimeCounter);
  exampleCounter.countOneUp(longTimeCounter);

  exampleCounter.countOneUp(shortTimeCounter);
  exampleCounter.countOneUp(toolIdentNumber);
  exampleCounter.countOneUp(toolIdentNumber);

  exampleCounter.countOneUp(toolIdentNumber);

  //exampleCounter.update(longTimeCounter, 11);
  exampleCounter.printDebugInformation();
  //exampleCounter.resetStoreLocation();
  //long ValueFromGetValue = exampleCounter.getValue(longTimeCounter);
  //Serial.println(ValueFromGetValue);
  delay(4000);
}
