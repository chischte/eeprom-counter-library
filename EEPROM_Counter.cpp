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

EEPROM_Counter::EEPROM_Counter(int eepromSize, int numberOfValues)
{
  // READ WHERE THE VALUES ARE STORED:
  _numberOfValues = numberOfValues;
  _maxStoreLocation = (eepromSize - 1) - (numberOfValues * 4);    //eeprom address is 0 indexed
}

void EEPROM_Counter::initialize()
{
  int valueRead;
  int valueAddress = 0;    // the storeLocation is always stored on EEPROM byte 0 and 1
  eeprom_read_block((void*) &valueRead, (void*) valueAddress, sizeof(valueRead));
  //.........................destination / source / size
  _storeLocation = valueRead;
  //IF STORELOCATION IS TO BIG OR TO SMALL, RESET IT TO 2
  //...this can happen with a new board only
  if (_storeLocation > _maxStoreLocation || _storeLocation < 2)
  {
    _storeLocation = 2;
    int newValue = _storeLocation;
    int valueAddress = 0;
    // the adress of the storeLocation is always stored on byte 0 and 1;
    eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
    //..........................source / destination / size}
    _numberOfWriteCycles = eepromRead(_storeLocation);
  }
}

void EEPROM_Counter::countOneUp(int valueNumber)
{
  int valueAddress = calculateAddress(valueNumber);
  long storedValue = eepromRead(valueAddress);
  long newValue = storedValue + 1;
  eepromWrite(newValue, valueAddress);
}

void EEPROM_Counter::update(int valueNumber, long newValue)
{
  int valueAddress = calculateAddress(valueNumber);
  //CHECK IF VALUE HASN'T BEEN ASSIGNED YET
  long storedValue = eepromRead(valueAddress);
  if (newValue != storedValue)
  {
    eepromWrite(newValue, valueAddress);
  }
}

long EEPROM_Counter::getValue(int valueNumber)
{
  int valueAddress = calculateAddress(valueNumber);
  long valueRead = eepromRead(valueAddress);
  return valueRead;
}

void EEPROM_Counter::setAllZero()
{
  for (int i = 0; i < _numberOfValues; i++)
  {
    int valueAddress = calculateAddress(i);
    eepromWrite(0, valueAddress);
  }
}

int EEPROM_Counter::calculateAddress(int valueNumber)
{
  int addressNumber = _storeLocation + 4 + valueNumber * 4;
  //shift 4 bytes for the writeCounter and 4 bytes for every new value
  return addressNumber;
}

long EEPROM_Counter::eepromRead(int sourceAddress)
{
  long valueRead;
  eeprom_read_block((void*) &valueRead, (void*) sourceAddress, sizeof(valueRead));
  return valueRead;
}

void EEPROM_Counter::eepromWrite(long newValue, int destinationAddress)
{
  eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
  eepromMonitorWriteCycles();
}

void EEPROM_Counter::eepromMonitorWriteCycles()
{
  _numberOfWriteCycles++;
  if (_numberOfWriteCycles > 10)
  {
    eepromMoveStorageLocation();
    _numberOfWriteCycles = 0;
    int newValue = _numberOfWriteCycles;
    int destinationAddress = _storeLocation;
    //eepromWrite(_numberOfWriteCycles, _storeLocation);
    eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));

  }
}

void EEPROM_Counter::eepromMoveStorageLocation()
{
  //CALCULATE NEW STORE LOCATION:
  int moveSizeInBytes = 4 + _numberOfValues * 4; //the long for the writeCounter is included
  int newStoreLocation = _storeLocation + moveSizeInBytes;
  if (newStoreLocation > _maxStoreLocation)
  {
    newStoreLocation = 2;
  }
  //COPY VALUES FROM OLD TO NEW LOCATION:
  for (int i = 0; i < _numberOfValues; i++)
  {
    int oldValueAddress = calculateAddress(i);
    long storedValue = eepromRead(oldValueAddress);
    int newValueAddress = newStoreLocation + 4 + (i * 4);

    long newValue = storedValue;
    int destinationAddress = newValueAddress;
    eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
  }
  //Serial.println("WROTE");
  _storeLocation = newStoreLocation;
  int newValue = _storeLocation;
  int valueAddress = 0;
  // the adress of the storeLocation is always stored on byte 0 and 1;
  eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
  //.......................... source / destination / size}
}

void EEPROM_Counter::printDebugInformation()
{
  Serial.print("writeCount:");
  Serial.print(_numberOfWriteCycles);
  Serial.print(" noOfValues: ");
  Serial.print(_numberOfValues);
  Serial.print(" storLoc: ");
  Serial.println(_storeLocation);

  for (int i = 0; i < _numberOfValues; i++)
  {
    Serial.print("Value Number: ");
    Serial.print(i);

    Serial.print("add:");
    int addressRead = calculateAddress(i);
    Serial.print(addressRead);

    Serial.print(" val: ");
    long valueRead = getValue(i);
    Serial.println(valueRead);
  }
  //*/
  ////Serial.println("");
}

//THIS FUNCTION PROBABLY CAN BE ERASED AFTER LIBRARY WORKS FINE:
void EEPROM_Counter::resetStoreLocation()
{
  int newValue = 2;
  int valueAddress = 0;
// the adress of the storeLocation is always stored on byte 0 and 1;
  eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
// source / destination / size}
  _storeLocation = newValue;
  Serial.println("VALUE RESETED !!!");
}
