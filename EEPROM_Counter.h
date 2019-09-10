/*
 * *****************************************************************************
   EEPROM_Counter.h
   Library to store long values on the non volatile arduino EEPROM
   Michael Wettstein
   September 2019, Zürich
 * *****************************************************************************
   The EEPROM memory has a specified lifetime of 100,000 write/erase cycles
   The storage location will be changed after 100,000 cycles
   Storage data Type unsigned long (4Bytes)
 * *****************************************************************************
 EEPROM USAGE
SLOT 0 stores the storelocation

storeLocation      stores the writecounter
storelocation1-n places where the values are stored  
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

        int _storeLocation;





};
#endif