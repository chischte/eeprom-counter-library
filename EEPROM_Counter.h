/*
 * *****************************************************************************
 EEPROM_Counter.h
 Library to store long values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 The EEPROM memory has a specified lifetime of 100,000 write/erase cycles
 The wear out the EEPROM memory evenly, the storage location will be changed every 10,000 write cycles
 Storage data Type unsigned long (4Bytes)
 * *****************************************************************************
 EEPROM USAGE
 SLOT 0 stores the storelocation

 storeLocation      stores the writecounter
 storelocation1-n places where the values are stored  
 * *****************************************************************************
 TODO:
 CHECK MOVE OF STORAGE LOCATION
 IN DEBUG PRINT WRITCOUNTER SHOWS RANDOM NUMBERS
 IN DEBUG VALUE 0 JUMPS TO VERY HIGH NUMBERS
 */

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter {
public:
    EEPROM_Counter(const int eepromSize, byte numberOfValues);

    void countOneUp(byte valueNumber);
    //... increases the value by one

    void update(byte valueNumber, long newValue);
    //... checks if value is new, store new value on eeprom
    //... counts writecycles and changes storage location before eeprom wears out

    void setAllZero();

    long getValue(byte valueNumber); // returns current value

    void printDebugInformation();

    void resetStoreLocation();

private:

    int _storeLocation;
    byte _numberOfValues;
    int calculateAddress(byte valueNumber);
    long eepromRead(int sourceAdress);
    void eepromWrite(long newValue, int destinationAddress);
    void eepromMonitorWriteCycles();
    void eepromMoveStorageLocation();
    long _numberOfWriteCycles;
    int _maxStoreLocation;

};
#endif
