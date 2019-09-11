/*
 * *****************************************************************************
 EEPROM_Counter.h
 Library to store long values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 The EEPROM memory has a specified lifetime of 100,000 write/erase cycles
 The wear out the EEPROM memory evenly, the storage location will be changed every 10,000 write cycles
 Storage data Type long (4Bytes)
 * *****************************************************************************
 EEPROM USAGE
 SLOT 0 stores the storelocation

 storeLocation      stores the writecounter
 storelocation1-n places where the values are stored  
 * *****************************************************************************
 TODO:
 CLEAN UP THE WHOLE CODE
 STORE EVERY THOUSANDS WRITE COUNT
 */

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter
{
public:
  //FUNTIONS:
  EEPROM_Counter(int eepromSize, int numberOfValues);
  void countOneUp(int valueNumber); // increases the value by one
  void update(int valueNumber, long newValue); // sets value
  void setAllZero();
  long getValue(int valueNumber); // returns current value
  void printDebugInformation(); // extensively prints what happens with the values

  // VARIABLES:
  //n.a.

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
