# sLiachdDingens-ESP8266

**🛩️ Einfache 5-Kanal Modellflug Beleuchtungssteuerung**

Eine ESP8266-basierte Beleuchtungssteuerung für Modellflugzeuge mit webbasierter Konfiguration und flexiblen Lichtprofilen.

## ✨ Features

### Hardware
- **5 Ausgänge**: 3x 500mA, 2x 1A Belastbarkeit
- **Doppelte Anschlüsse**: Je Port 2 Steckplätze verfügbar
- **Universelle Stromversorgung**: 1-3S LiPo mit automatischer Erkennung
- **Kompaktes Design**: Optimiert für den Einbau in Modellflugzeuge

### Software
- **3 LightProfile**: Verschiedene Beleuchtungsmodi
- **Standalone-Betrieb**: Funktioniert auch ohne RC-Anlage
- **Web-Interface**: Konfiguration über WLAN-AccessPoint im Browser
- **RC-Integration**: 
  - Kanal 1: LightProfile-Umschaltung (3-Pos-Schalter)
  - Kanal 2: LowPower/MaxPower-Modus (2-Pos-Schalter oder Taster)
- **Intelligente Akkuüberwachung**: 
  - LowBatt-Warnung
  - Konfigurierbarer CutOff-Schutz
- **Flexible Blinksequenzen**: 
  - Vorgefertigte Sequenzen
  - Frei konfigurierbare Muster
- **OTA-Updates**: Firmware-Updates über WLAN
- **Backup-Funktion**: Download und Sicherung der Einstellungen

## 🔧 Installation

### Hardware-Aufbau

> ⚠️ **Wichtiger Hinweis**: Der ESP8266 sollte idealerweise **vor dem Einlöten** mit der OTA-fähigen Firmware geflasht werden! Als SMD-Bauteil ist das Flashen nach dem Verlöten über die seriellen Pins deutlich schwieriger zugänglich.

**Empfohlene Reihenfolge:**
1. **ESP8266 vorbereiten**: Filesystem und Firmware flashen (siehe Software-Installation)
2. **ESP8266 einlöten**: Bereits vorbereiteten ESP8266 SMD-Bauteil einbauen
3. **Weitere SMD-Bauteile löten**: ULA2003A und andere oberflächenmontierte Komponenten
4. **Anschlusskabel montieren**: Abschließende Verkabelung

**Hauptkomponenten:**
- ESP8266 Mikrocontroller (SMD)
- ULA2003A Treiber-IC (SMD)
- Weitere SMD-Komponenten (siehe Schaltplan)

### Software-Installation

> ⚠️ **Wichtig**: Reihenfolge beachten!

**Initial-Flash (vor dem Einlöten stark empfohlen):**
1. **Filesystem Upload** - Beinhaltet die wichtige `HW.json` mit EEPROM-Initialwerten
2. **Firmware Upload**
3. **Funktionstest** durchführen
4. **ESP8266 auf die Platine löten**

**Alternative (nach dem Einlöten):**
- OTA-Updates über das Web-Interface möglich
- Serielle Programmierung über die Programmierpins (bei SMD deutlich schwieriger)

## 🚀 Erste Schritte

1. ESP8266 mit Strom versorgen
2. WLAN-AccessPoint suchen (sLiachdDingens-XXXX)
3. Mit dem AccessPoint verbinden
4. Browser öffnen und zur Konfigurationsseite navigieren
5. Grundeinstellungen vornehmen:
   - Akkuspannungen konfigurieren
   - RC-Kanäle zuweisen
   - LightProfile anpassen

## 📋 Technische Daten

| Spezifikation | Wert |
|---------------|------|
| Eingangsspannung | 1-3S LiPo (3.7V - 12.6V) |
| Ausgänge | 5 Kanäle |
| Ausgangsleistung | 3x 500mA, 2x 1A |
| Mikrocontroller | ESP8266 (SMD) |
| Konfiguration | Web-Interface über WLAN |
| RC-Kanäle | 2 (optional) |

## 🔄 Entwicklungsstatus

> **🚧 In Entwicklung**
> 
> Dieses Projekt befindet sich aktiv in der Entwicklung. Features können sich ändern und neue Funktionen werden kontinuierlich hinzugefügt.

## 🤝 Beitragen

Feedback, Bug-Reports und Verbesserungsvorschläge sind willkommen! Erstellen Sie gerne ein Issue oder einen Pull Request.

## 📄 Lizenz

Dieses Projekt steht unter der [GNU GPL v3](LICENSE).  
Copyright © 2025 V0giK

Alle Abwandlungen und Forks dieses Projekts müssen ebenfalls unter den Bedingungen der GNU GPL v3 stehen.

---

**Made with ❤️ für die Modellflug-Community**
