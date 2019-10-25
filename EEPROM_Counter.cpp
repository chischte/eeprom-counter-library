/*
 * *****************************************************************************
 EEPROM_Counter.cpp
 Library to store values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 */

#include "Arduino.h"
#include "EEPROM_Counter.h"

EEPROM_Counter::EEPROM_Counter(int eepromMinAddress, int eepromMaxAddress, int numberOfValues) {
  // READ WHERE THE VALUES ARE STORED:
  _eepromMinAddress = eepromMinAddress;
  _minValueStorelocation = _eepromMinAddress + 2; //the first to bytes are reserved for the library
  _numberOfValues = numberOfValues;
  _maxStoreLocation = (eepromMaxAddress) - (_bytesForWriteCounter + (numberOfValues * 4));
  int valueRead;
  int valueAddress = _eepromMinAddress; // the storeLocation is always stored on the first two bytes
  eeprom_read_block((void*) &valueRead, (void*) valueAddress, sizeof(valueRead));
  //-------------------------destination--------source--------size
  _storeLocation = valueRead;
  // IF STORELOCATION IS TO BIG OR TO SMALL, RESET IT TO 2
  // this can happen with a new board only
  if (_storeLocation > _maxStoreLocation || _storeLocation < _minValueStorelocation) {
    _storeLocation = _minValueStorelocation;
    int newValue = _storeLocation;
    int valueAddress = _eepromMinAddress; // the adress of the storeLocation is always stored on the lowest 2 bytes;

    eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
  }

  _numberOfWriteCycles = eepromRead(_storeLocation);
  // IF numberOfWriteCycles IS NEGATIVE, RESET IT TO 0
  // this can happen with a new board only
  if (_numberOfWriteCycles < 0) {
    _numberOfWriteCycles = 0;
    int newValue = 0;
    int valueAddress = _storeLocation; // the adress of the storeLocation is always stored on the lowest 2 bytes;

    eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
  }
}

void EEPROM_Counter::countOneUp(int valueNumber) {
  int valueAddress = calculateAddress(valueNumber);
  long storedValue = eepromRead(valueAddress);
  long newValue = storedValue + 1;
  eepromWrite(newValue, valueAddress);
}

void EEPROM_Counter::set(int valueNumber, long newValue) {
  int valueAddress = calculateAddress(valueNumber);
  long storedValue = eepromRead(valueAddress);
  // CHECK IF VALUE HASN'T BEEN ASSIGNED YET: 
  if (newValue != storedValue) {
    eepromWrite(newValue, valueAddress);
  }
}

long EEPROM_Counter::getValue(int valueNumber) {
  int valueAddress = calculateAddress(valueNumber);
  long valueRead = eepromRead(valueAddress);
  return valueRead;
}

void EEPROM_Counter::setAllZero() {
  for (int i = 0; i < _numberOfValues; i++) {
    int valueAddress = calculateAddress(i);
    eepromWrite(0, valueAddress);
  }
}

int EEPROM_Counter::calculateAddress(int valueNumber) {
  int addressNumber = _storeLocation + _bytesForWriteCounter + valueNumber * 4;
  //shift 4 bytes for the writeCounter and 4 bytes for every new value
  return addressNumber;
}

long EEPROM_Counter::eepromRead(int sourceAddress) {
  long valueRead;
  eeprom_read_block((void*) &valueRead, (void*) sourceAddress, sizeof(valueRead));
  //-------------------------destination--------source---------size
  return valueRead;
}

void EEPROM_Counter::eepromWrite(long newValue, int destinationAddress) {
  eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
  //--------------------------source------------destination---------size}
  eepromMonitorWriteCycles();
}

void EEPROM_Counter::eepromMonitorWriteCycles() {
  _numberOfWriteCycles++;
  if (_numberOfWriteCycles > 10000) {
    eepromMoveStorageLocation();
    _numberOfWriteCycles = 0;
    int newValue = _numberOfWriteCycles;
    int destinationAddress = _storeLocation;
    eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
    _storeWriteCounterCounter = 0;
  }

  _storeWriteCounterCounter++;
  if (_storeWriteCounterCounter >= 100)  // every n-writes the writecountervalue will be stored
          {
    long newValue = _numberOfWriteCycles;
    int destinationAddress = _storeLocation;
    eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
    _storeWriteCounterCounter = 0;
  }
}

void EEPROM_Counter::eepromMoveStorageLocation() {
  // CALCULATE NEW STORE LOCATION:
  int moveSizeInBytes = _bytesForWriteCounter + _numberOfValues * 4; //the long for the writeCounter is included
  int newStoreLocation = _storeLocation + moveSizeInBytes;
  if (newStoreLocation > _maxStoreLocation) {
    newStoreLocation = _minValueStorelocation;
  }
  // COPY VALUES FROM OLD TO NEW LOCATION:
  for (int i = 0; i < _numberOfValues; i++) {
    int oldValueAddress = calculateAddress(i);
    long storedValue = eepromRead(oldValueAddress);
    int newValueAddress = newStoreLocation + _bytesForWriteCounter + (i * 4);

    long newValue = storedValue;
    int destinationAddress = newValueAddress;
    eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
  }
  // CHANGE STORELOCATION ADDRESS:
  _storeLocation = newStoreLocation;
  int newValue = _storeLocation;
  int valueAddress = _eepromMinAddress;
  // the adress of the storeLocation is always stored on byte 0 and 1;
  eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
  //--------------------------source------------destination---size}
}

void EEPROM_Counter::printDebugInformation() {
  char debugInfo[100];
  Serial.println("*******************************");
  sprintf(debugInfo, "STORELOCATION: %d     ", _storeLocation);
  Serial.print(debugInfo);
  sprintf(debugInfo, "WRITE COUNT: %ld", _numberOfWriteCycles);
  Serial.println(debugInfo);

  for (int i = 0; i < _numberOfValues; i++) {
    sprintf(debugInfo, "Value Number: %d  ", i);
    Serial.print(debugInfo);
    sprintf(debugInfo, "Value Adress: %d  ", calculateAddress(i));
    Serial.print(debugInfo);
    sprintf(debugInfo, "Value %d = %ld", i, getValue(i));
    Serial.println(debugInfo);
  }
  Serial.println("*******************************");
}
