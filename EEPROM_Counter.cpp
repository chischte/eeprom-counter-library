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

EEPROM_Counter::EEPROM_Counter(int eepromSize, int numberOfValues) {
    // READ WHERE THE VALUES ARE STORED:
    int valueRead;
    int valueAddress = 0;
    eeprom_read_block((void*) &valueRead, (void*) valueAddress, sizeof(valueRead));
    // destination / source / size
    // the storeLocation is always stored on EEPROM byte 0 and 1
    _storeLocation = valueRead;
    _maxStoreLocation = (eepromSize - 1) - (numberOfValues * 4);    //eeprom address is 0 indexed
    if (_storeLocation > _maxStoreLocation || _storeLocation < 2) //can happen with a new board
            {
        _storeLocation = 2;
        int newValue = 2;
        int valueAddress = 0;
        // the adress of the storeLocation is always stored on byte 0 and 1;
        eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
        // source / destination / size}

    }

    // READ THE NUMBER OF WRITE CYCLES:
    _numberOfWriteCycles = eepromRead(_storeLocation);
    _numberOfValues = numberOfValues;
}

void EEPROM_Counter::countOneUp(int valueNumber) {
    int valueAddress = calculateAddress(valueNumber);
    long storedValue = eepromRead(valueAddress);
    long newValue = storedValue + 1;
    eepromWrite(newValue, valueAddress);
}

void EEPROM_Counter::update(int valueNumber, long newValue) {
    int valueAddress = calculateAddress(valueNumber);
    //CHECK IF VALUE HASN'T BEEN ASSIGNED YET
    long storedValue = eepromRead(valueAddress);
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
    int addressNumber = _storeLocation + 4 + valueNumber * 4;
    //shift 4 bytes for the writeCounter and 4 bytes for every new value
    return addressNumber;
}

long EEPROM_Counter::eepromRead(int sourceAddress) {
    long valueRead;
    eeprom_read_block((void*) &valueRead, (void*) sourceAddress, sizeof(valueRead));
    return valueRead;
}

void EEPROM_Counter::eepromWrite(long newValue, int destinationAddress) {

    eeprom_write_block((void*) &newValue, (void*) destinationAddress, sizeof(newValue));
    eepromMonitorWriteCycles();
}

void EEPROM_Counter::eepromMonitorWriteCycles() {
    _numberOfWriteCycles++;
    if (_numberOfWriteCycles > 10) {
        eepromMoveStorageLocation();
        _numberOfWriteCycles = 0;
        eepromWrite(_numberOfWriteCycles, _storeLocation);
    }
}

void EEPROM_Counter::eepromMoveStorageLocation() {
    //CALCULATE NEW STORE LOCATION:
    int moveSizeInBytes = _numberOfValues * 4; //the long for the writeCounter is included
    int newStoreLocation = _storeLocation + moveSizeInBytes;
    /*
    if (newStoreLocation > _maxStoreLocation) {
     newStoreLocation = 2;
     }
*/
    //COPY VALUES FROM OLD TO NEW LOCATION:
    for (int i = 0; i < _numberOfValues; i++) {
        int oldValueAddress = calculateAddress(i);
        long storedValue = eepromRead(oldValueAddress);
        int newValueAddress = newStoreLocation + 4 + (i * 4);
        eepromWrite(storedValue, newValueAddress);
    }
    //Serial.println("WROTE");
    _storeLocation = newStoreLocation;
    int newValue = _storeLocation;
    int valueAddress = 0;
    // the adress of the storeLocation is always stored on byte 0 and 1;
    eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
    // source / destination / size}
    //Serial.println("NEW");
}

void EEPROM_Counter::printDebugInformation() {

    Serial.print("LOC ");
    Serial.println(_storeLocation);
    /*
    Serial.print(" WRITECOUNT: ");
    Serial.print(_numberOfWriteCycles);
    Serial.print(" NUMBER OF VALUES: ");
    Serial.println(_numberOfValues);
    Serial.println("");
    */
    /*
     for (int i = 0; i < _numberOfValues; i++) {

     Serial.print("#");
     Serial.print(i);

     Serial.print(" addr:");
     int addressRead = calculateAddress(i);
     Serial.print(addressRead);

     Serial.print(" value: ");
     long valueRead = getValue(i);
     Serial.println(valueRead);
     }
     */
    //Serial.println("");
}

void EEPROM_Counter::resetStoreLocation() {
    int newValue = 2;
    int valueAddress = 0;
// the adress of the storeLocation is always stored on byte 0 and 1;
    eeprom_write_block((void*) &newValue, (void*) valueAddress, sizeof(newValue));
// source / destination / size}
    _storeLocation = newValue;
    Serial.println("VALUE RESETED !!!");
}
