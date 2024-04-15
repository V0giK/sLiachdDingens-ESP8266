// EepromFunctions.cpp

#include "EepromFunctions.h"
#include <EEPROM.h>
#include "Settings.h"

EepromFunctions::EepromFunctions(){
    //
}

bool EepromFunctions::eraseEeprom() {
    bool bRet = true;
    // Reset EEPROM bytes to '0' for the length of the gBattCalibFactor
    EEPROM.begin(4095);
    for (uint8_t i = 0; i < sizeof(_Eeprom); i++) {
        EEPROM.write(i, 0);
    }
    delay(200);
    bRet = EEPROM.commit();
    EEPROM.end();

    if(bRet){
        // Factor mit 1.0 initialisieren
        bRet = saveEeprom();
    }

    return bRet;
}

bool EepromFunctions::saveEeprom() {
  bool bRet = true;
  if(_Eeprom.battCalibFactor == 0.0){
    _Eeprom.battCalibFactor = 1.0;
  }
  // Save configuration from RAM into EEPROM
  EEPROM.begin(4095);
  EEPROM.put(0, _Eeprom);
  delay(200);
  bRet = EEPROM.commit();               // Only needed for ESP8266 to get data written
  EEPROM.end();                         // Free RAM copy of structure

  return bRet;
}

EepromFunctions::EEPROM_Data EepromFunctions::loadEeprom() {
  // Loads configuration from EEPROM into RAM
  EEPROM.begin(sizeof(_Eeprom));
  EEPROM.get(0, _Eeprom);
  EEPROM.end();
  delay(200);

  return _Eeprom;
}

EepromFunctions::EEPROM_Data EepromFunctions::getEeprom() {
    return _Eeprom;
}

void EepromFunctions::setEeprom(EepromFunctions::EEPROM_Data& eeprom) {
    _Eeprom = eeprom;
}

// Anzahl Ports
uint8_t EepromFunctions::getPortCount(){
  return _Eeprom.portCnt;
}
// Anzahl Profile
uint8_t EepromFunctions::getProfileCount(){
  return _Eeprom.profileCnt;
}
// Anzahl Slots je Durchlauf
uint8_t EepromFunctions::getSlotCount(){
  return _Eeprom.slotCnt;
}


float EepromFunctions::getBattCalibFactor(){
  return _Eeprom.battCalibFactor;
}

void EepromFunctions::setBattCalibFactor(float factor){
  if(factor == 0.0)
    factor = 1.0;
  _Eeprom.battCalibFactor = factor;
}
