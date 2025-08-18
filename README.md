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

**Lötreihenfolge:**
1. SMD-Bauteile zuerst löten
2. Anschlusskabel montieren

**Hauptkomponenten:**
- ESP8266 Mikrocontroller
- ULA2003A Treiber-IC
- Weitere Komponenten (siehe Schaltplan)

### Software-Installation

> ⚠️ **Wichtig**: Reihenfolge beachten!

1. **Filesystem Upload** - Beinhaltet die wichtige `HW.json` mit EEPROM-Initialwerten
2. **Firmware Upload**
3. **Fertig!** 😉

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
| Mikrocontroller | ESP8266 |
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
