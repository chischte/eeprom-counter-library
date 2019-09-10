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