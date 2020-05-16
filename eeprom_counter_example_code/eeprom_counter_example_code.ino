#include <EEPROM_Counter.h> //https://github.com/chischte/eeprom-counter-library.git

// NAME THE VALUES TO BE STORED ON EEPROM BY ENTERING THEM IN THE LIST BELOW:
enum counter {
  longTimeCounter, // example value name
  shortTimeCounter, // example value name
  toolIdentNumber, // example value name
  somethingElse, // example value name
  endOfEnum // add additional values before this one...
  // ...this has to be the last one!
};
int numberOfValues = endOfEnum;

// DEFINE THE EEPROM SIZE OF YOUR BOARD:
// EEPROM size Arduino Nano/Uno: 1024 bytes (ATmega328P) -> maxAddress = 1023
// EEPROM size Arduino Mega:     4096 bytes (ATmega2560) -> maxAddress = 4095

// IT IS POSSIBLE TO USE ONLY A RANGE OF THE EEPROM
// THIS CAN BE NECESSARY IF MORE THAN ONE FUNCTION REQUIRES ACCES TO A PART OF THE EEPROM
// THE ASSIGNED SIZE HAS TO BE AT LEAST (numberOfValues*4+6)bytes

int eepromMinAddress = 500; // has to be 0 or bigger
int eepromMaxAddress =
    1000; // has to be at least one smaller than the EEPROM size of the processor used

// CREATE AN INSTANCE OF THE LIBRARY CLASS:
EEPROM_Counter exampleCounter;

void setup() {
  exampleCounter.setup(eepromMinAddress, eepromMaxAddress, numberOfValues);
  Serial.begin(115200);
  Serial.println("EXIT SETUP");
}

void loop() {

  //exampleCounter.setAllZero(); //sets all values to zero

  for (int i = 0; i < 5; i++) {
    exampleCounter.count_one_up(longTimeCounter);
  }
  exampleCounter.count_one_up(shortTimeCounter);

  exampleCounter.count_one_up(toolIdentNumber);

  exampleCounter.set(somethingElse, 112233);

  long ValueFromGetValue = exampleCounter.get_value(longTimeCounter);
  Serial.print("Value from getValue Function: ");
  Serial.println(ValueFromGetValue);
  exampleCounter.print_debug_information();
  delay(1000);
}
