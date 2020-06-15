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

void EEPROM_Counter::setup(int eeprom_min_address, int eeprom_max_address, int number_of_values) {
  // READ WHERE THE VALUES ARE STORED:
  _eeprom_min_address = eeprom_min_address;
  _min_value_store_location =
      _eeprom_min_address + 2; //the first to bytes are reserved for the library
  _number_of_values = number_of_values;
  _max_store_location = (eeprom_max_address) - (_bytes_for_write_counter + (number_of_values * 4));
  int value_read;
  int value_address =
      _eeprom_min_address; // the storeLocation is always stored on the first two bytes
  eeprom_read_block((void *)&value_read, (void *)value_address, sizeof(value_read));
  //-------------------------destination--------source--------size
  _store_location = value_read;
  // IF STORELOCATION IS TO BIG OR TO SMALL, RESET IT TO 2
  // this can happen with a new board only
  if (_store_location > _max_store_location || _store_location < _min_value_store_location) {
    _store_location = _min_value_store_location;
    int new_value = _store_location;
    int value_address =
        _eeprom_min_address; // the adress of the storeLocation is always stored on the lowest 2 bytes;

    eeprom_write_block((void *)&new_value, (void *)value_address, sizeof(new_value));
    //--------------------------source------------destination---size}
  }

  _number_of_write_cycles = eeprom_read(_store_location);
  // IF numberOfWriteCycles IS NEGATIVE, RESET IT TO 0
  // this can happen with a new board only
  if (_number_of_write_cycles < 0) {
    _number_of_write_cycles = 0;
    int new_value = 0;
    int value_address =
        _store_location; // the adress of the storeLocation is always stored on the lowest 2 bytes;

    eeprom_write_block((void *)&new_value, (void *)value_address, sizeof(new_value));
    //--------------------------source------------destination---size}
  }
}
void EEPROM_Counter::count_one_up(int value_number) {
  int value_address = calculate_address(value_number);
  long stored_value = eeprom_read(value_address);
  long new_value = stored_value + 1;
  eeprom_write(new_value, value_address);
}

void EEPROM_Counter::set_value(int value_number, long new_value) {
  int value_address = calculate_address(value_number);
  long stored_value = eeprom_read(value_address);
  // CHECK IF VALUE HASN'T BEEN ASSIGNED YET:
  if (new_value != stored_value) {
    eeprom_write(new_value, value_address);
  }
}

long EEPROM_Counter::get_value(int value_number) {
  int value_address = calculate_address(value_number);
  long value_read = eeprom_read(value_address);
  return value_read;
}

void EEPROM_Counter::set_all_zero() {
  for (int i = 0; i < _number_of_values; i++) {
    int value_address = calculate_address(i);
    eeprom_write(0, value_address);
  }
}

int EEPROM_Counter::calculate_address(int value_number) {
  int address_number = _store_location + _bytes_for_write_counter + value_number * 4;
  //shift 4 bytes for the writeCounter and 4 bytes for every new value
  return address_number;
}

long EEPROM_Counter::eeprom_read(int source_address) {
  long value_read;
  eeprom_read_block((void *)&value_read, (void *)source_address, sizeof(value_read));
  //-------------------------destination--------source---------size
  return value_read;
}

void EEPROM_Counter::eeprom_write(long new_value, int destination_address) {
  eeprom_write_block((void *)&new_value, (void *)destination_address, sizeof(new_value));
  //--------------------------source------------destination---------size}
  eeprom_monitor_write_cycles();
}

void EEPROM_Counter::eeprom_monitor_write_cycles() {
  _number_of_write_cycles++;
  if (_number_of_write_cycles > 10000) {
    eeprom_move_storage_location();
    _number_of_write_cycles = 0;
    int new_value = _number_of_write_cycles;
    int destination_address = _store_location;
    eeprom_write_block((void *)&new_value, (void *)destination_address, sizeof(new_value));
    //--------------------------source------------destination---size}
    _store_write_counter_counter = 0;
  }

  _store_write_counter_counter++;
  if (_store_write_counter_counter >= 100) // every n-writes the writecountervalue will be stored
  {
    long new_value = _number_of_write_cycles;
    int destination_address = _store_location;
    eeprom_write_block((void *)&new_value, (void *)destination_address, sizeof(new_value));
    //--------------------------source------------destination---size}
    _store_write_counter_counter = 0;
  }
}

void EEPROM_Counter::eeprom_move_storage_location() {
  // CALCULATE NEW STORE LOCATION:
  int moveSizeInBytes =
      _bytes_for_write_counter + _number_of_values * 4; //the long for the writeCounter is included
  int new_store_location = _store_location + moveSizeInBytes;
  if (new_store_location > _max_store_location) {
    new_store_location = _min_value_store_location;
  }
  // COPY VALUES FROM OLD TO NEW LOCATION:
  for (int i = 0; i < _number_of_values; i++) {
    int old_value_address = calculate_address(i);
    long stored_value = eeprom_read(old_value_address);
    int new_value_address = new_store_location + _bytes_for_write_counter + (i * 4);

    long new_value = stored_value;
    int destination_address = new_value_address;
    eeprom_write_block((void *)&new_value, (void *)destination_address, sizeof(new_value));
    //--------------------------source------------destination---size}
  }
  // CHANGE STORELOCATION ADDRESS:
  _store_location = new_store_location;
  int new_value = _store_location;
  int value_address = _eeprom_min_address;
  // the adress of the storeLocation is always stored on byte 0 and 1;
  eeprom_write_block((void *)&new_value, (void *)value_address, sizeof(new_value));
  //--------------------------source------------destination---size}
}

void EEPROM_Counter::print_debug_information() {
  char debug_info[100];
  Serial.println("*******************************");
  sprintf(debug_info, "STORELOCATION: %d     ", _store_location);
  Serial.print(debug_info);
  sprintf(debug_info, "WRITE COUNT: %ld", _number_of_write_cycles);
  Serial.println(debug_info);

  for (int i = 0; i < _number_of_values; i++) {
    sprintf(debug_info, "Value Number: %d  ", i);
    Serial.print(debug_info);
    sprintf(debug_info, "Value Adress: %d  ", calculate_address(i));
    Serial.print(debug_info);
    sprintf(debug_info, "Value %d = %ld", i, get_value(i));
    Serial.println(debug_info);
  }
  Serial.println("*******************************");
}
