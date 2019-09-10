/*
 * *****************************************************************************
 * EEPROM_Counter.h
 * Library to store long values on the non volatile arduino EEPROM
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
 * The EEPROM memory has a specified lifetime of 100,000 write/erase cycles
 * The storage location will be changed after 100,000 cycles
 * Storage data Type unsigned long (4Bytes)
 * *****************************************************************************
 */
 

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter
{
	public:
		EEPROM_Counter(const int eepromSize, byte eepromNumberOfValues);
		
		void countOneUp(byte valueNumber);
		//... increases the value by one
		
		void update(byte valueNumber, long newValue);
		//... checks if value is new, store new value on eeprom
		//... counts writecycles and changes storage location before eeprom wears out 
		
		void setAllZero();
		
		long getValue(byte valueNumber); // returns current value
	

	
	private:
	
	long _storedValue[20]; //array to store all values [max number of values that can be stored]
	
	int _storeLocation;
	
	
	
	
	
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