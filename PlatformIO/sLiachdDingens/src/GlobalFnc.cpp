// GlobalFnc.cpp

#include "GlobalFnc.h"
#include "globalvariables.h"
#include "Settings.h"

/**
 * Index für dynamische mehrdimensionale  Arrays erstellen
 * - also alle die mit malloc(...) erstellt wurden
*/
uint16_t GlobalFnc::MA(uint8_t portCnt, uint8_t profileCnt) {
    return portCnt + ( profileCnt * gEeprom.getPortCount());
};

/**
 * Index für dynamische mehrdimensionale  Arrays erstellen
 * - also alle die mit malloc(...) erstellt wurden
*/
uint16_t GlobalFnc::MA(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt) {
    return portCnt + ( profileCnt * gEeprom.getPortCount()) + ( slotCnt * (gEeprom.getPortCount()*gEeprom.getProfileCount()));
};
