Hauptfunktionen des sLiachdDingens
1. Mehrkanal-Lichtsteuerung
Steuerung von bis zu 5 Lichtports (z.B. Strobe, Beacon, Landelicht, Nav).
Jeder Port kann mehrere Profile und Sequenzen besitzen (z.B. verschiedene Blinkmuster).
Profile werden über einen RC-Schalter (CH1) ausgewählt (z.B. 3-Stufenschalter).
2. LowPower-Modus
Über einen zweiten RC-Kanal (CH2) kann die Leistung aller Ports reduziert werden (z.B. für Nachtflug).
Umschaltung zwischen 100% und konfigurierbarem LowPower-Wert (z.B. 10%).
3. Sequenz- und Presetverwaltung
Lichtmuster (Sequenzen) können für jeden Port und jedes Profil individuell eingestellt werden.
Vordefinierte Presets (z.B. verschiedene Strobe- und Beacon-Muster) können geladen und auf Ports angewendet werden.
4. Webbasierte Konfiguration
Konfiguration erfolgt über eine Weboberfläche (ESP8266 als Access Point).
Einstellungen für Ports, Profile, Sequenzen, Presets, RC-Kanäle, Akku-Grenzwerte und Access Point können im Browser angepasst werden.
Upload/Download von Konfigurationsdateien (Backup & Restore).
5. Akkuüberwachung
Messung der Batteriespannung und automatische Erkennung der Zellenzahl.
Warnung bei Unterspannung: Leistung wird zyklisch reduziert (Blinken).
CutOff-Funktion: Bei kritischer Spannung wird das System in den DeepSleep versetzt.
Kalibrierung der Spannungsmessung über die Weboberfläche.
6. EEPROM/FS Verwaltung
Hardwaredaten (Portanzahl, Profile, Slots, Kalibrierfaktor) werden im EEPROM gespeichert.
Konfigurations- und Sequenzdaten werden im Dateisystem (LittleFS) abgelegt.
7. OTA Firmware Update
Unterstützung für Over-the-Air Updates direkt über die Weboberfläche.
8. Reset und Neustart
Webbasierter Hardware-Reset des ESP8266 möglich.
9. Benutzerfreundliche Oberfläche
Dynamische HTML-Generierung für alle Konfigurationsbereiche.
Modal-Dialoge für Status- und Fehlermeldungen.
Menüstruktur für schnellen Zugriff auf alle Funktionen.

Technische Details
Hardware: ESP8266, PWM-Ausgänge für LEDs, analoge Spannungsmessung.
Software: C++ (Arduino/PlatformIO), JavaScript/jQuery für die Weboberfläche.
Datenhaltung: JSON-Dateien für Konfiguration, Sequenzen und Presets.
RC-Auswertung: Interrupt-gesteuerte Messung der Pulsweiten für Schalterpositionen.
Modularer Aufbau: Trennung von Hardwarefunktionen, Webserver, Konfiguration und Logik.

Weboberfläche
index.html: Konfiguration der Lichtports und Sequenzen.
portcurrent.html: Einstellung der Port-Leistung und LowPower-Modus.
dspPresets.html: Übersicht und Auswahl der Presets.
config_channel.html: RC-Kanäle und Boot-Positionen.
config_batt.html: Akku-Grenzwerte und Kalibrierung.
config_ap.html: Access Point Einstellungen.
imexport.html: Backup und Restore der Konfigurationsdateien.

Zusammengefasst:
Die RC Lichtsteuerung bietet eine flexible und leistungsfähige Lösung zur Steuerung von Modellbau-Lichtsystemen mit umfangreicher Web-Konfiguration, RC-Integration, Akkuüberwachung und Preset-Verwaltung. Alle Einstellungen können komfortabel über den Browser vorgenommen und gesichert werden.