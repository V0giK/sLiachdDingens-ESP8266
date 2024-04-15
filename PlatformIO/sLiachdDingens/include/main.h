// main.h
#pragma once
#include <Arduino.h>

/* Prototypen-dekalration */

void mapSlot2Port(uint8_t);
//void startPrev(bool);
void setAllLightPorts(uint8_t);
void setAllLightPortsOff();
long seqSlotVal(uint8_t, uint8_t, uint8_t);
void handleRcChannels();
void IRAM_ATTR RcCalcChange();
void IRAM_ATTR RcLowPwrChange();
bool getPrev();
void setPrev(bool);
