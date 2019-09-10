/*
 * *****************************************************************************
 * EEPROM_Counter.h
 * Library to store values on the non volatile arduino EEPROM
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
 */

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter
{
	public:
		EEPROM_Counter(const int eepromSize, byte eepromNumberOfValues);
	
	private:
	
};
#endif







/*
class Debounce
{
  public:
    Debounce(const byte BUTTON_PIN);
    bool requestButtonState();

  private:
    bool _debouncedButtonState;
    bool _currentButtonState;
    bool _debounceTimerSet = 0;
    
	byte _BUTTON_PIN;
	int _debounceTime = 10;
    
	unsigned long _prevTime;
};

#endif
*/