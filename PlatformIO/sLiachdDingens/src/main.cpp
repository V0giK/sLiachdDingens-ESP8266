// maim.cpp

#include <LittleFS.h>
//#include <Wire.h>

#include "main.h"
#include "Settings.h"
#include "GlobalFnc.h"
#include "RcSwitchHelper.h"


#include "globalvariables.h"
//#include "I2C_Anything.h"
#include "WiFi_WebServerHandling.h"


/* Input Pins */
#define PIN_AP D0   // AP aktivieren
#define PIN_CH_PR D1   // CH1 Profile schalten (Servoeingang)
#define PIN_CH_LP D2   // CH2 für LowPower mode
#define PIN_BATT A0 // Spannungsüberwachung Batt
/* Output Pins */
#define PIN_APACT D4  // AP aktiv (LED)
#define PIN_O1 D5
#define PIN_O2 D6
#define PIN_O3 D7
#define PIN_O4 D8
#define PIN_O5 3 // ist RX
const uint8_t outPinAr[] = {PIN_O1, PIN_O2, PIN_O3, PIN_O4, PIN_O5};

//bool bStartPrev = false;  // Loop anhalten bis alle Variablen neu bestückt sind (INI neu laden)

/* Slot Zeit */
unsigned long prevMillis = 0;   // speichert die letzte zeit des Slots
const long interval = 60;       // Interval bis nächster Slot beginnt (milliseconds)

/* RC Signal auswerten */
volatile ITR_STRUCT ITR_Ch1;
volatile ITR_STRUCT ITR_Ch2;

/* RC Schalter */
RcSwitchHelper gCH1;
RcSwitchHelper gCH2;

bool _bPrev = false; // Loop anhalten bis alle Variablen neu bestückt sind (.json neu laden)
WiFi_WebServerHandling webServer;

/***********************************************************************************************************/


void setup() {

  #ifdef DEV_SERIAL_OUT
  Serial.begin(SERIAL_SPEED);
  #endif
  if(!LittleFS.begin()) {
    #ifdef DEV_SERIAL_OUT
    Serial.println("LittleFS Mount nicht gelungen");
    #endif
  }

    /* EEPROM lesen */
  //gEeprom.loadEeprom(); // aus EEPROM
  // EEPROM beschreiben falls neue HardwareDaten vorhanden sind (HW.json)
  if(LittleFS.exists(PATH_HW)) {
    gJsonHelper.readDeleteEraseJsonHW(gEeprom);
    gEeprom.saveEeprom();

    // TODO : oder weiter machen?
    delay(200);
    ESP.restart();
  } else {
    gEeprom.loadEeprom(); // aus EEPROM
  }

  // JsonHelper initialisieren
  gJsonHelper.begin(gEeprom.getPortCount(), gEeprom.getProfileCount(), gEeprom.getSlotCount());
  
  // Config laden
  gJsonHelper.loadJsonConfig();
  // Sequence laden
  gJsonHelper.loadJsonSequence();

  // BattHelper initialisieren
  gBatt.begin(PIN_BATT, gJsonHelper.getConfigBattWarningVolt(), gJsonHelper.getConfigBattCutOffVolt(), gEeprom.getBattCalibFactor(), BATT_WARN_LOW);

  // RC Schalter initialisieren
  gCH1.begin((gJsonHelper.getSequence()->ch1Boot == 2)?1:gJsonHelper.getSequence()->ch1Boot, (gJsonHelper.getSequence()->ch1Boot == 2));
  gCH2.begin((gJsonHelper.getSequence()->ch2Boot == 2)?1:gJsonHelper.getSequence()->ch2Boot, (gJsonHelper.getSequence()->ch2Boot == 2));
  ITR_Ch1.bIsToggleSwitsch = gCH1.isToggleSwitch();
  ITR_Ch2.bIsToggleSwitsch = gCH2.isToggleSwitch();
  
  
  /* WiFi deaktivieren */
  // Wird im Constructor erledigt
  //webServer.disableWifi();

  // Serial Pin's auf GPIO tauschen  
  //pinMode(1, FUNCTION_3);  //GPIO 1 (TX) swap the pin to a GPIO.
  pinMode(3, FUNCTION_3);  //GPIO 3 (RX) swap the pin to a GPIO.

  // PinMode
  pinMode(PIN_AP, INPUT_PULLUP); // kein interner PULLUP vorhanden nur PULLDOWN
  pinMode(PIN_CH_PR, INPUT);
  pinMode(PIN_CH_LP, INPUT);
  pinMode(PIN_BATT, INPUT);
  pinMode(PIN_APACT, OUTPUT);
  pinMode(PIN_O1, OUTPUT);
  pinMode(PIN_O2, OUTPUT);
  pinMode(PIN_O3, OUTPUT);
  pinMode(PIN_O4, OUTPUT);
  pinMode(PIN_O5, OUTPUT);
  //attachInterrupt(PIN_SV, RcChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_CH_PR), RcCalcChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_CH_LP), RcLowPwrChange, CHANGE);
  
    // I2C Starten
  #ifndef DEV_SERIAL_OUT

  /*
    pinMode(1, FUNCTION_3);  //GPIO 1 (TX) swap the pin to a GPIO.
    pinMode(3, FUNCTION_3);  //GPIO 3 (RX) swap the pin to a GPIO.
    delay(200);
    Wire.begin(1, 3);
  */

    /*
     * I2C Scannen
     * - prüfen ob für Adresse ein .json vorhanden ist
     * - wenn ja Adresse in Array aufnehmen
     */
    /*
    byte i = 0;
    for(byte adr=8; adr<120; adr++){
      if(i >= EXPAND_MAX)
        break;
      Wire.beginTransmission(adr);
      if(0 == Wire.endTransmission()){
        // noch prüfen ob Exp??.json vorhanden
        if(LittleFS.exists(PATH_JSON + String("/Exp") + String(adr) + ".json")){
          gExpandAdresses[i] = adr;
          i++;
        }
      }
    }
    */
  #endif

  // Vorbelegung für den Start
  gLowPwr = gJsonHelper.getSequence()->lowPwrPercent;
  

  /*
  Wire.beginTransmission(0);
  Wire.write("W1");
  Wire.write(gPosCH1);
  Wire.write("W2");
  Wire.write(gPosCH2);
  Wire.endTransmission();
  */
  

  // Access Point aktivieren wenn Taster gedrückt
  digitalWrite(PIN_APACT, HIGH);     // AP LED off

  if(!digitalRead(PIN_AP) || DBG_AP_ON) {
    webServer.AP_starten();
    digitalWrite(PIN_APACT, LOW);  // AP LED on
  }else{
    //WiFi.mode(WIFI_SHUTDOWN);
  }

  // warten bis RC Anlage gebunden sein müsste
  delay(500);

}

void loop() {

  /* Position CH1 */
  /* Position CH2 */
  handleRcChannels();



  unsigned long curMillis = millis();
  if (curMillis - prevMillis >= interval && !getPrev()) {
    prevMillis = curMillis;

    /* VBatt */
    if(giCurSlot % 4 == 1) { // 20 Slots % 4 = 0-3; nach jeweils 4 Slots erneut prüfen (4x 0,06sec = 0,24sec)

      // Batterie Daten aktualisieren
      gBatt.refresh();
      
      // Unterspannung
      // CutOff
      if(gBatt.isCutOff()){
        setAllLightPortsOff();
        mapSlot2Port(0);
        delay(200);
        ESP.deepSleep(ESP.deepSleepMax());
      }
    }
    /* end - VBatt */

    if(giCurSlot >= gEeprom.getSlotCount()) giCurSlot = 0;
    
    // Ausgabe an die Ports
    mapSlot2Port(giCurSlot);

    #ifndef DEV_SERIAL_OUT
    /*
    Wire.beginTransmission(0);
    Wire.write("S"); Wire.write(giCurSlot);
    Wire.endTransmission();
    */
    #endif

    giCurSlot++;
    yield();
  }
  //ESP.wdtDisable(); 

}

/**
 * Slots an die Pins ausgeben
 */
void mapSlot2Port(uint8_t slot){
  for(byte i=0; i<sizeof(outPinAr); i++){
    analogWrite(outPinAr[i], seqSlotVal(i, gCH1.getPosition(), slot)*(gLowPwr/100.0)*(gJsonHelper.getSequence()->pwr[GlobalFnc::MA(i, gCH1.getPosition())]/100.0)*(gBatt.getWarnPwr()/100.0));
  }
}

/**
 * Slot Value auf Ausgabewert umrechnen
*/
long seqSlotVal(uint8_t port, uint8_t profile, uint8_t slot){
  return map(gJsonHelper.getSequence()->seq[GlobalFnc::MA(port, profile, slot)], 0, 8, 0, 1023);
}

/**
 * Alle LightPorts auf Parameter setzen
 * 
 * Parameter:
 * val - gewünschter Wert (0-1023)
 */
void setAllLightPorts(uint8_t val){
  for(uint8_t x = 0; x < gEeprom.getPortCount(); x++) {
    for(uint8_t prof=0; prof < gEeprom.getProfileCount(); prof++) {
      for(uint8_t y = 0; y < gEeprom.getSlotCount(); y++) {
          gJsonHelper.getSequence()->seq[GlobalFnc::MA(x, prof, y)] = val;
      }
    }
  }
}
void setAllLightPortsOff() {
  setAllLightPorts(0);
}


void handleRcChannels() {
  /* Position CH1 */
  if(ITR_Ch1.bHasBooking){
    gCH1.setPosition(ITR_Ch1.newVal);
    ITR_Ch1.bHasBooking = false;
  }

  /* Position CH2 */
  // LowPower Mode
  if(ITR_Ch2.bHasBooking) {
    gCH2.setPosition(ITR_Ch2.newVal);
    // LowPower festlegen
    if(gCH2.getPosition() == 0){
      gLowPwr = gJsonHelper.getSequence()->lowPwrPercent;
    } else {
      gLowPwr = 100;
    }
    ITR_Ch2.bHasBooking = false;
  }
}

/**
 * Daten für Vorschau vorbereiten und dadurch in loop() nichts ausführen
 */
void setPrev(bool val){
    _bPrev = val;
}
bool getPrev() {
    return _bPrev;
}



//--------ISR--------
void RcCalcChange(){
  noInterrupts();
  
  if(digitalRead(PIN_CH_PR) == HIGH) {
    ITR_Ch1.startTime = micros();
  } else if(ITR_Ch1.startTime && !ITR_Ch1.bHasBooking) {

    ITR_Ch1.pulseLength = (volatile int)(micros() - ITR_Ch1.startTime);

    // 1000 - 2000 (1500 ist Mittelstellung)
    if(ITR_Ch1.pulseLength > 900) {
      if(ITR_Ch1.pulseLength <= 1333) {
        ITR_Ch1.newVal = 0;
      } else if(ITR_Ch1.pulseLength <= 1666) {
        ITR_Ch1.newVal = 1;
      } else {
        ITR_Ch1.newVal = 2;
      }
      ITR_Ch1.bHasBooking = true;
    }

    ITR_Ch1.startTime = 0;
  }
  
  interrupts();
}

void RcLowPwrChange(){
  static uint8_t tasterNormal = 0, tasterNow = 0;
  static bool tgFirst = true;
  
  noInterrupts();
  if(digitalRead(PIN_CH_LP) == HIGH) {
    ITR_Ch2.startTime = micros();
  } else if(ITR_Ch2.startTime && !ITR_Ch2.bHasBooking) {
    ITR_Ch2.pulseLength = (volatile int)(micros() - ITR_Ch2.startTime);

    // Position
    if(ITR_Ch2.pulseLength > 900) {
      if( ITR_Ch2.pulseLength < 1550 )
        ITR_Ch2.newVal = 0;
      else
        ITR_Ch2.newVal = 1;
    }

    if(ITR_Ch2.bIsToggleSwitsch){
        if(tgFirst){
            tgFirst = false;
            tasterNormal = ITR_Ch2.newVal;
            tasterNow = ITR_Ch2.newVal;
        } else {
            // Toggle
            //if(_tasterNow != _tasterNormal && _tasterNow != gNewPosCH2){ // Toggle on release button
            if(tasterNow == tasterNormal && tasterNow != ITR_Ch2.newVal){ // Toggle on press button
                ITR_Ch2.bHasBooking = true;
            }
            tasterNow = ITR_Ch2.newVal;
        }

    } else {
        ITR_Ch2.bHasBooking = true;
    }


    ITR_Ch2.startTime = 0;
  }
  interrupts();
}
