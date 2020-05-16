/*
 * *****************************************************************************
 EEPROM_Counter.cpp
 Library to store values on the non volatile arduino EEPROM
 Michael Wettstein
 September 2019, Zürich
 * *****************************************************************************
 */

#include "EEPROM_Counter.h"
#include "Arduino.h"

EEPROM_Counter::EEPROM_Counter() {}

void EEPROM_Counter::setup(int eepromMinAddress, int eepromMaxAddress, int numberOfValues) {
  // READ WHERE THE VALUES ARE STORED:
  _eeprom_min_address = eepromMinAddress;
  _min_value_store_location =
      _eeprom_min_address + 2; //the first to bytes are reserved for the library
  _number_of_values = numberOfValues;
  _max_store_location = (eepromMaxAddress) - (_bytes_for_write_counter + (numberOfValues * 4));
  int valueRead;
  int valueAddress =
      _eeprom_min_address; // the storeLocation is always stored on the first two bytes
  eeprom_read_block((void *)&valueRead, (void *)valueAddress, sizeof(valueRead));
  //-------------------------destination--------source--------size
  _store_location = valueRead;
  // IF STORELOCATION IS TO BIG OR TO SMALL, RESET IT TO 2
  // this can happen with a new board only
  if (_store_location > _max_store_location || _store_location < _min_value_store_location) {
    _store_location = _min_value_store_location;
    int newValue = _store_location;
    int valueAddress =
        _eeprom_min_address; // the adress of the storeLocation is always stored on the lowest 2 bytes;

    eeprom_write_block((void *)&newValue, (void *)valueAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
  }

  _number_of_write_cycles = eeprom_read(_store_location);
  // IF numberOfWriteCycles IS NEGATIVE, RESET IT TO 0
  // this can happen with a new board only
  if (_number_of_write_cycles < 0) {
    _number_of_write_cycles = 0;
    int newValue = 0;
    int valueAddress =
        _store_location; // the adress of the storeLocation is always stored on the lowest 2 bytes;

    eeprom_write_block((void *)&newValue, (void *)valueAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
  }
}
void EEPROM_Counter::count_one_up(int valueNumber) {
  int valueAddress = calculate_address(valueNumber);
  long storedValue = eeprom_read(valueAddress);
  long newValue = storedValue + 1;
  eeprom_write(newValue, valueAddress);
}

void EEPROM_Counter::set_value(int valueNumber, long newValue) {
  int valueAddress = calculate_address(valueNumber);
  long storedValue = eeprom_read(valueAddress);
  // CHECK IF VALUE HASN'T BEEN ASSIGNED YET:
  if (newValue != storedValue) {
    eeprom_write(newValue, valueAddress);
  }
}

long EEPROM_Counter::get_value(int valueNumber) {
  int valueAddress = calculate_address(valueNumber);
  long valueRead = eeprom_read(valueAddress);
  return valueRead;
}

void EEPROM_Counter::set_all_zero() {
  for (int i = 0; i < _number_of_values; i++) {
    int valueAddress = calculate_address(i);
    eeprom_write(0, valueAddress);
  }
}

int EEPROM_Counter::calculate_address(int valueNumber) {
  int addressNumber = _store_location + _bytes_for_write_counter + valueNumber * 4;
  //shift 4 bytes for the writeCounter and 4 bytes for every new value
  return addressNumber;
}

long EEPROM_Counter::eeprom_read(int sourceAddress) {
  long valueRead;
  eeprom_read_block((void *)&valueRead, (void *)sourceAddress, sizeof(valueRead));
  //-------------------------destination--------source---------size
  return valueRead;
}

void EEPROM_Counter::eeprom_write(long newValue, int destinationAddress) {
  eeprom_write_block((void *)&newValue, (void *)destinationAddress, sizeof(newValue));
  //--------------------------source------------destination---------size}
  eeprom_monitor_write_cycles();
}

void EEPROM_Counter::eeprom_monitor_write_cycles() {
  _number_of_write_cycles++;
  if (_number_of_write_cycles > 10000) {
    eeprom_move_storage_location();
    _number_of_write_cycles = 0;
    int newValue = _number_of_write_cycles;
    int destinationAddress = _store_location;
    eeprom_write_block((void *)&newValue, (void *)destinationAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
    _store_write_counter_counter = 0;
  }

  _store_write_counter_counter++;
  if (_store_write_counter_counter >= 100) // every n-writes the writecountervalue will be stored
  {
    long newValue = _number_of_write_cycles;
    int destinationAddress = _store_location;
    eeprom_write_block((void *)&newValue, (void *)destinationAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
    _store_write_counter_counter = 0;
  }
}

void EEPROM_Counter::eeprom_move_storage_location() {
  // CALCULATE NEW STORE LOCATION:
  int moveSizeInBytes =
      _bytes_for_write_counter + _number_of_values * 4; //the long for the writeCounter is included
  int newStoreLocation = _store_location + moveSizeInBytes;
  if (newStoreLocation > _max_store_location) {
    newStoreLocation = _min_value_store_location;
  }
  // COPY VALUES FROM OLD TO NEW LOCATION:
  for (int i = 0; i < _number_of_values; i++) {
    int oldValueAddress = calculate_address(i);
    long storedValue = eeprom_read(oldValueAddress);
    int newValueAddress = newStoreLocation + _bytes_for_write_counter + (i * 4);

    long newValue = storedValue;
    int destinationAddress = newValueAddress;
    eeprom_write_block((void *)&newValue, (void *)destinationAddress, sizeof(newValue));
    //--------------------------source------------destination---size}
  }
  // CHANGE STORELOCATION ADDRESS:
  _store_location = newStoreLocation;
  int newValue = _store_location;
  int valueAddress = _eeprom_min_address;
  // the adress of the storeLocation is always stored on byte 0 and 1;
  eeprom_write_block((void *)&newValue, (void *)valueAddress, sizeof(newValue));
  //--------------------------source------------destination---size}
}

void EEPROM_Counter::print_debug_information() {
  char debugInfo[100];
  Serial.println("*******************************");
  sprintf(debugInfo, "STORELOCATION: %d     ", _store_location);
  Serial.print(debugInfo);
  sprintf(debugInfo, "WRITE COUNT: %ld", _number_of_write_cycles);
  Serial.println(debugInfo);

  for (int i = 0; i < _number_of_values; i++) {
    sprintf(debugInfo, "Value Number: %d  ", i);
    Serial.print(debugInfo);
    sprintf(debugInfo, "Value Adress: %d  ", calculate_address(i));
    Serial.print(debugInfo);
    sprintf(debugInfo, "Value %d = %ld", i, get_value(i));
    Serial.println(debugInfo);
  }
  Serial.println("*******************************");
}
