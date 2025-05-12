# sLiachdDingens-ESP8266
Einfache 5-Kanal Modellflug Beleuchtungssteuerung

> **under developmet**


- 5 Ports (3x 500mA, 2x 1A)
- je Port 2 Steckplätze
- 1-3 S Lipo (Auto Erkennung)
- 3 LightProfile
- Betrieb auch ohne RC Anlage möglich
- Konfiguration über Browser (WLAN-AccessPoint)
	- Akku Überwachung (LowBatt Warnung, CutOff) - Spannungen einstellbar
	- RC Kanal 1 - LightProfile schalten (3 PosSchalter)
	- RC Kanal 2 - LowPower/MaxPower (2 PosSchalter oder Taster)
	- Startposition der RC Kanäle einstellbar (erst nach erreichen dieser Position kann diese geändert werden)
	- Auswählbare Blinksequencen (alle Ausgänge)
	- frei einstellbare Blinksequencen (alle Ausgänge)
	- Firmware Update möglich (OTA)
	- Download der Einstellungen möglich
	- Sicherung des Profils

---

## Aufbau

### Lötreihenfolge
1. SMD Bauteile
2. Kabel

---
    
### Initial Firmware Upload

> 1. Filesystem Upload - **inkl. HW.json** - (Initialwerte für EEPROM)
> 2. Firmware Upload
> 3. fertig😉

---

### Komponenten

- ESP8266
- ULA2003A
- ...


---

## Lizenz

Dieses Projekt steht unter der [GNU GPL v3](LICENSE).  
Copyright © 2025 V0giK

Alle Abwandlungen und Forks dieses Projekts müssen ebenfalls
unter den Bedingungen der GNU GPL v3 stehen.
