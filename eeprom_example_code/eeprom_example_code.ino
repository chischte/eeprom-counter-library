#include <EEPROM_Counter.h>

// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
//777777777777777777777777777 the list can have between 1 and 255 value entries.
enum counter
{
  longTimeCounter,    // example value name
  shortTimeCounter,   // example value name
  toolIdentNumber,    // example value name
  somethingElse,      // example value name
  endOfEnum           // add additional values before this one...
                      // ...this has to be the last one!
};
int numberOfValues = endOfEnum;

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P)
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560)
int eepromSize = 1024;

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Counter exampleCounter(eepromSize, numberOfValues);

void setup()
{
  Serial.begin(115200);
  Serial.println("EXIT SETUP");
}

void loop()
{

  //exampleCounter.setAllZero(); //sets all values to zero

  for (int i = 0; i < 5; i++)
  {
    exampleCounter.countOneUp(longTimeCounter);
  }
  exampleCounter.countOneUp(shortTimeCounter);

  exampleCounter.countOneUp(toolIdentNumber);

  exampleCounter.set(somethingElse, 112233);

  long ValueFromGetValue = exampleCounter.getValue(longTimeCounter);
  Serial.print("Value from getValue Function: ");
  Serial.println(ValueFromGetValue);
  exampleCounter.printDebugInformation();
  //delay(1000);

}
