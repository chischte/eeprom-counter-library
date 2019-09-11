
#include <EEPROM_Counter.h>



// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
// the list can have between 1 and 255 value entries.
enum counter {
  longTimeCounter,  // example value name
  shortTimeCounter, // example value name
  toolIdentNumber,  // example value name
  endOfEnum //dont touch this one
};
int numberOfValues = endOfEnum;

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P)
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560)
int eepromSize = 1024;

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Counter exampleCounter(eepromSize, numberOfValues);
//EEPROM_Counter exampleCounter(eepromSize, 4);
// The values in the brackets define the eeprom size and the number of values to be stored.


void setup() {
  Serial.begin(115200);
}

void loop() {

  //exampleCounter.setAllZero();

  exampleCounter.countOneUp(longTimeCounter);
  //exampleCounter.countOneUp(longTimeCounter);

  exampleCounter.countOneUp(shortTimeCounter);

  //exampleCounter.countOneUp(toolIdentNumber);
  //exampleCounter.countOneUp(toolIdentNumber);
  exampleCounter.countOneUp(toolIdentNumber);

  //exampleCounter.update(longTimeCounter, 11);
  //exampleCounter.update(-1, 11);
  exampleCounter.printDebugInformation();
  //exampleCounter.resetStoreLocation();
  //long ValueFromGetValue = exampleCounter.getValue(longTimeCounter);
  //Serial.println(ValueFromGetValue);
  delay(2000);
}
