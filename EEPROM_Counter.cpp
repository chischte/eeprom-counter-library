/*
 * *****************************************************************************
 * EEPROM_Counter.cpp
 * Library to store values on the non volatile arduino EEPROM
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
 */

#include "Arduino.h"
#include "EEPROM_Counter.h"


EEPROM_Counter::EEPROM_Counter(const int eepromSize, byte eepromNumberOfValues)
{
	//Read all Values from eeprom an write them into the array (SRAM)
	
	
	 // READ WHERE THE VALUES ARE STORED:
    int storeLocation;
	eeprom_read_block((void*) &storeLocation, (void*) 0, sizeof(2)); // destination / source / size
    _storeLocation=storeLocation;
	
	
	// READ ALL VALUES FROME EEPROM AND STORE THEM IN THE ARRAY
	for(int i; i<eepromNumberOfValues;i++)
	{
		int sourceAddress = _storeLocation+4+i*4; //shift 4 bytes for the writecounter and 4 bytzes for every new value
		long valueRead;
	
	eeprom_read_block((void*) &valueRead, (void*) sourceAddress, sizeof(4)); // destination / source / size
	//STORE THE CURRENT VALUE IN THE ARRAY:
	//_storedValue[i]=valueRead;
	}
    
}


void EEPROM_Counter::countOneUp(byte valueNumber)
{
	Serial.println(_storedValue[1]);
}


void EEPROM_Counter::update(byte valueNumber, long newValue) 
{

/*
//IF STORELOCATION IS AT THE END OF LIFECYCLE, ASSIGN A NEW STORELOCATION
  if (EEPROM_WriteCounter >= 100000)
  {
    //ASSIGN A NEW STORELOCATION:
    storeLocation = storeLocation + 4 + (4 * STORESLOTS); // 4 bytes for the write_counter + 4 bytes for each store slot
    if (storeLocation >= (4095 - 40))
    {
      storeLocation = 2;
    }
    eeprom_write_block((void*) &storeLocation, (void*) 0, 2); // source / destination / size
    EEPROM_WriteCounter = 0;
	
		eeprom_read_block((void*) &EEPROM_WriteCounter, (void*) storeLocation, sizeof(4)); // destination / source / size
    
	
  }


*/
  }
  
  long EEPROM_Counter::getValue(byte valueNumber)
  {
  }


void EEPROM_Counter::setAllZero()
{
}





/*
Debounce::Debounce(const byte BUTTON_PIN)
{
  _BUTTON_PIN = BUTTON_PIN;
}
//***************************************************************************
//LIBRARY FUNCTIONS:
//***************************************************************************

bool Debounce::requestButtonState()
{

  _currentButtonState = digitalRead(_BUTTON_PIN);

  if (_currentButtonState != _debouncedButtonState && _debounceTimerSet == false)
  {
    _prevTime = millis();
    _debounceTimerSet = true;
  }

  if (millis() - _prevTime > _debounceTime)
  {
    _debounceTimerSet = false; // debouncetimer can be reset
    if (_currentButtonState != _debouncedButtonState)
    {
      _debouncedButtonState = _currentButtonState;
    }

    return _currentButtonState;
  }
}
*/