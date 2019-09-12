/*
 * *****************************************************************************
 EEPROM_Counter.h
 Library to store long values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 The EEPROM memory has a specified lifetime of 100,000 write/erase cycles
 To wear out the EEPROM memory evenly, the storage location will be changed periodically
 Storage data Type long (4Bytes)
 * *****************************************************************************
 EEPROM USAGE
 address of the _storeLocation_: 0
 address of the writecounter: _storeLocation
 address of value-n: _storelocation + n*sizeof(long)  
 * *****************************************************************************
 TODO:
 CLEAN UP THE WHOLE CODE
 CHANGE BYTES BACK TO BYTES (NOT INT)
 */

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter
{
public:
  // FUNTIONS:
  EEPROM_Counter(int eepromSize, int numberOfValues);
  void countOneUp(int valueNumber); // increases the value by one
  void set(int valueNumber, long newValue); // sets value
  long getValue(int valueNumber); // returns current value
  void printDebugInformation(); // extensively prints what happens with the values
  void setAllZero(); // resets all counters
 
 // VARIABLES:
  // n.a.

private:
  // FUNCTIONS:
  long eepromRead(int sourceAdress);
  void eepromWrite(long newValue, int destinationAddress);
  void eepromMonitorWriteCycles();
  void eepromMoveStorageLocation();
  int calculateAddress(int valueNumber);

  // VARIABLES:
  int _storeLocation;
  int _maxStoreLocation;
  int _storeWriteCounterCounter;
  int _numberOfValues;
  long _numberOfWriteCycles;

};
#endif
