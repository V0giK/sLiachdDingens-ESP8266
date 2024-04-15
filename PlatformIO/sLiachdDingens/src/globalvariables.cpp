// globalvariables.cpp

#include "globalvariables.h"
#include "Settings.h"

EepromFunctions gEeprom;
JsonHelper2 gJsonHelper;
BattHelper gBatt;


/* Slot Variablen */
uint8_t giCurSlot = 0;

/* Leistung an den Ausgängen in % */
uint8_t gLowPwr = 50;

