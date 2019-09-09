/*
 * *****************************************************************************
 * EEPROM_Counter.h
 * Library to store values on the arduino EEPROM memory
 * Michael Wettstein
 * September 2019, Zürich
 * *****************************************************************************
 */

#ifndef EEPROM_Counter_h
#define EEPROM_Counter_h

#include "Arduino.h"

class EEPROM_Counter
{
public:
  Cylinder(int pin);
  void stroke(int push_time, int release_time);
  void toggle();
  void set(bool set_state);
  bool request_state();
  bool stroke_completed();

private:
  int _pin;
  bool _state;
  unsigned long _prev_time;
  bool _stroke_completed = true;
};

#endif
