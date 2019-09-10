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

EEPROM_Counter::EEPROM_Counter(const int eepromSize, byte numberOfValues)
{
  // READ WHERE THE VALUES ARE STORED:
  int valueRead;
  eeprom_read_block((void*) &valueRead, (void*) 0, sizeof(valueRead));
  // destination / source / size
  // the storeLocation is always stored on EEPROM byte 0 and 1
  _storeLocation = valueRead;
  _maxStoreLocation = eepromSize - numberOfValues * 4;
  if (_storeLocation > _maxStoreLocation || _storeLocation < 2) //can happen with a new board
  {
    resetStoreLocation();
  }

  // READ THE NUMBER OF WRITE CYCLES:
  long _eepromNumberOfWriteCycles = eepromRead(_storeLocation);
  _numberOfValues = numberOfValues;
}

void EEPROM_Counter::countOneUp(byte valueNumber)
{
  int valueAddress = calculateAddress(valueNumber);
  long storedValue = eepromRead(valueAddress);
  long newValue = ++storedValue;
  eepromWrite(newValue, valueAddress);
}

void EEPROM_Counter::update(byte valueNumber, long newValue)
{
  int valueAddress = calculateAddress(valueNumber);
  //CHECK IF VALUE HASN'T BEEN ASSIGNED YET
  long storedValue = eepromRead(valueAddress);
  if (newValue != storedValue)
  {
    eepromWrite(newValue, valueAddress);
  }
}

long EEPROM_Counter::getValue(byte valueNumber)
{
  int valueAddress = calculateAddress(valueNumber);
  long valueRead = eepromRead(valueAddress);
  return valueRead;
}

void EEPROM_Counter::setAllZero()
{
  for (byte i = 0; i < _numberOfValues; i++)
  {
    int valueAddress = calculateAddress(i);
    eepromWrite(0, valueAddress);
  }
}

int EEPROM_Counter::calculateAddress(byte valueNumber)
{
  int addressNumber = _storeLocation + 4 + (valueNumber * 4);
  //shift 4 bytes for the writeCounter and 4 bytes for every new value
  return addressNumber;
}

long EEPROM_Counter::eepromRead(int sourceAddress)
{
  long valueRead;
  eeprom_read_block((void*) &valueRead, (void*) sourceAddress,
      sizeof(valueRead));
  return valueRead;
}

void EEPROM_Counter::eepromWrite(long newValue, int destinationAddress)
{

  eeprom_write_block((void*) &newValue, (void*) destinationAddress,
      sizeof(newValue));
  eepromMonitorWriteCycles();
}

void EEPROM_Counter::eepromMonitorWriteCycles()
{
  _numberOfWriteCycles++;
  if (_numberOfWriteCycles > 20)
  {
    eepromMoveStorageLocation();
    _numberOfWriteCycles = 0;
    eepromWrite(_numberOfWriteCycles, _storeLocation);
  }
}

void EEPROM_Counter::eepromMoveStorageLocation()
{
  //CALCULATE NEW STORE LOCATION:
  int moveSizeInBytes = _numberOfValues * 4; //the long for the writeCounter is included
  int newStoreLocation = _storeLocation + moveSizeInBytes;
  if (newStoreLocation > _maxStoreLocation)
  {
    newStoreLocation = 2;
  }

  //COPY VALUES FROM OLD TO NEW LOCATION:
  for (byte i = 0; i < _numberOfValues; i++)
  {
    int oldValueAddress = calculateAddress(i);
    long storedValue = eepromRead(oldValueAddress);
    int newValueAddress = newStoreLocation + 4 + (i * 4);
    eepromWrite(storedValue, newValueAddress);
  }
}

void EEPROM_Counter::printDebugInformation()
{
  char debugInfo[100];
  sprintf(debugInfo, "STORELOCATION: %d", _storeLocation); //%u unsigned int
  Serial.println(debugInfo);

  sprintf(debugInfo, "WRITECOUNTER:  %ld", _numberOfWriteCycles);
  Serial.println(debugInfo);

  for (byte i = 0; i < _numberOfValues; i++)
  {
    long valueRead = getValue(i);
    sprintf(debugInfo, "VALUE %d = %ld", i, valueRead);
    Serial.println(debugInfo);
  }
  Serial.println("");
}

void EEPROM_Counter::resetStoreLocation()
{
  int newValue = 2;
  int valueAddress = 0;
  // the adress of the storeLocation is always stored on byte 0 and 1;
  eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
  // source / destination / size}
}
