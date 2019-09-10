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

EEPROM_Counter::EEPROM_Counter(const int eepromSize,
        byte eepromNumberOfValues) {
    // READ WHERE THE VALUES ARE STORED:
    int storeLocation;
    eeprom_read_block((void*) &storeLocation, (void*) 0, 2);
    // destination / source / size
    _storeLocation = storeLocation;
}

void EEPROM_Counter::countOneUp(byte valueNumber) {
    int valueAddress = calculateAddress(valueNumber);
    eeprom_read_block((void*) &valueRead, (void*) sourceAddress, 4);
    // destination / source / size
    eeprom_write_block((void*) &newValue, (void*) destinationAddress, 4);
}

void EEPROM_Counter::update(byte valueNumber, long newValue) {

    //.....FIRST CHECK IF VALUE HASN'T BEEN ASSIGNED YET!!!

    int destinationAddress = calculateAddress(valueNumber);

    eeprom_write_block((void*) &newValue, (void*) destinationAddress, 4);
    // source / destination / size

    /*
     //IF STORELOCATION IS AT THE END OF LIFECYCLE, ASSIGN A NEW STORELOCATION
     if (EEPROM_WriteCounter >= 100000)
     {
     //ASSIGN A NEW STORELOCATION:
     storeLocation = storeLocation + 4 + (4 * STORESLOTS); // 4 bytes for the write_counter + 4 bytes for each store slot
     if (storeLocation >= (4095 - 40))
     {
     storeLocation = 2;
     }
     eeprom_write_block((void*) &storeLocation, (void*) 0, 2); // source / destination / size
     EEPROM_WriteCounter = 0;

     eeprom_read_block((void*) &EEPROM_WriteCounter, (void*) storeLocation, sizeof(4)); // destination / source / size


     }


     */
}

long EEPROM_Counter::getValue(byte valueNumber) {
    int sourceAddress = calculateAddress(valueNumber);

    int destinationAddress = sourceAddress;
    long valueRead;

    long newValue = -100;

    //eeprom_write_block((void*) &newValue, (void*) destinationAddress, 4);
    // source / destination / size

    eeprom_read_block((void*) &valueRead, (void*) sourceAddress, 4);
    // destination / source / size

    return valueRead;
}

void EEPROM_Counter::setAllZero() {
}

int EEPROM_Counter::calculateAddress(byte valueNumber) {
    int addressNumber = _storeLocation + 4 + (valueNumber * 4);
    //shift 4 bytes for the writecounter and 4 bytes for every new value
    return addressNumber;
}

