# sLiachdDingens – ESP8266 🛩️💡

**Einfache 5-Kanal Modellflug Beleuchtungssteuerung**  
ESP8266-basierte Lichtsteuerung für Modellflugzeuge mit webbasierter Konfiguration und flexiblen Profilen.

---

## 🚀 Hauptfunktionen

| Funktion                    | Beschreibung                                                                                  |
|-----------------------------|----------------------------------------------------------------------------------------------|
| **Mehrkanal-Lichtsteuerung**| Bis zu 5 Lichtports (z.B. Strobe, Beacon, Landelicht, Nav). Profile & Sequenzen pro Port.    |
| **LowPower-Modus**          | Per RC-Kanal Leistung aller Ports reduzieren (z.B. Nachtflug), Umschaltung 100%/LowPower.   |
| **Sequenz- & Presetverwaltung** | Individuelle Lichtmuster & vordefinierte Presets laden und anwenden.                      |
| **Webbasierte Konfiguration**| ESP8266 als Access Point: Konfiguration bequem im Browser möglich.                          |
| **Akkuüberwachung**         | Batteriespannung messen, automatische Zellenzahl-Erkennung, Warnung & CutOff.                |
| **EEPROM/FS Verwaltung**    | Hardwaredaten im EEPROM, Konfiguration/Sequenzen im LittleFS.                               |
| **OTA Firmware Update**     | Direkte Updates über die Weboberfläche.                                                     |
| **Reset & Neustart**        | Webbasierter Hardware-Reset des ESP8266.                                                    |
| **Benutzerfreundliche Oberfläche** | Dynamische HTML, Modal-Dialoge und intuitive Menüstruktur.                             |

---

## 🛠️ Technische Details

- **Hardware:** ESP8266, PWM-Ausgänge für LEDs, analoge Spannungsmessung
- **Software:** C++ (Arduino/PlatformIO), JavaScript/jQuery für Weboberfläche
- **Datenhaltung:** JSON-Dateien für Konfig, Sequenzen & Presets
- **RC-Auswertung:** Interrupt-gesteuerte Messung der Pulsweiten
- **Modularer Aufbau:** Trennung von Hardwarefunktionen, Webserver, Konfiguration & Logik

---

## 🌐 Weboberfläche – Konfigurationsseiten

| Seite             | Beschreibung                       |
|-------------------|------------------------------------|
| `index.html`      | Port-Konfiguration & Sequenzen     |
| `portcurrent.html`| Leistung & LowPower-Modus          |
| `dspPresets.html` | Preset-Übersicht & Auswahl         |
| `config_channel.html` | RC-Kanäle & Boot-Positionen |
| `config_batt.html`| Akku-Grenzwerte & Kalibrierung     |
| `config_ap.html`  | Access Point Einstellungen         |
| `imexport.html`   | Backup & Restore                   |

---

## 📦 Zusammengefasst

Die RC Lichtsteuerung bietet eine **flexible und leistungsfähige Lösung** zur Modellbau-Lichtsystem-Steuerung:

- Umfangreiche webbasierte Konfigurationsmöglichkeiten
- RC-Integration für Profile & LowPower
- Akkuüberwachung mit Warn- und Schutzfunktionen
- Backup, Restore & OTA-Updates

---

## 📚 Weitere Infos

- **Schaltplan & Hardwareaufbau:** Siehe Projektdateien
- **Firmware-Flash:** Vor Einbau des ESP8266 empfohlen!  
- **Lizenz:** GNU GPL v3

---

**Made with ❤️ für die Modellflug-Community!**
