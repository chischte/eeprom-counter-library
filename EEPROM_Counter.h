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
 address of the writecounter: _storeLocation (4bytes)
 address of value-n: _storelocation + n*sizeof(long)  
 * *****************************************************************************
 TODO:
 CLEAN UP THE WHOLE CODE
 */

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter {
public:
  // FUNTIONS:
  EEPROM_Counter();
  void setup(int eeprom_min_address, int eeprom_max_address, int number_of_values);
  void count_one_up(int value_number); // increases the value by one
  void set(int value_number, long new_value); // sets value
  long get_value(int value_number); // returns current value
  void print_debug_information(); // extensively prints what happens with the values
  void set_all_zero(); // resets all counters

  // VARIABLES:
  // n.a.

private:
  // FUNCTIONS:
  long eeprom_read(int sourceAdress);
  void eeprom_write(long new_value, int destination_address);
  void eeprom_monitor_write_cycles();
  void eeprom_move_storage_location();
  int calculate_address(int value_number);

  // VARIABLES:
  byte _bytes_for_write_counter = 4;
  int _store_location;
  int _min_value_store_location;
  int _eeprom_min_address;
  int _max_store_location;
  int _store_write_counter_counter;
  int _number_of_values;
  long _number_of_write_cycles;
};
#endif
