# Default OTA – ESP8266/ESP32 🔄✨

**Einfaches Over-the-Air (OTA) Firmware-Update Modul für ESP8266 und ESP32 mit PlatformIO**  
Dieses Projekt ermöglicht schnelle und unkomplizierte Firmware-Updates direkt über das Netzwerk – ohne USB!

---

## 🚀 Hauptfunktionen

| Funktion           | Beschreibung                                                                 |
|--------------------|------------------------------------------------------------------------------|
| **OTA-Updates**    | Firmware bequem per WiFi über Web-Browser oder PlatformIO hochladen          |
| **Kompatibel**     | Funktioniert mit ESP8266 und ESP32                                            |
| **Einfache Integration** | Minimaler Beispielcode, sofort einsatzbereit                        |
| **Sicher**         | Passwortschutz für die OTA-Schnittstelle (optional)                          |
| **Statusanzeige**  | Webbasierte Rückmeldung zum Update-Fortschritt                               |

---

## 🛠️ Installation & Nutzung

1. **Projekt mit PlatformIO öffnen**  
2. WLAN-Zugangsdaten (`SSID`, `Password`) in der `src/main.cpp` oder `platformio.ini` anpassen  
3. OTA-Endpunkt (z.B. `/update`) im Browser öffnen:  
   - `http://<IP-Adresse>/update`
4. Firmware (*.bin) auswählen und hochladen  
5. Gerät startet automatisch neu nach erfolgreichem Update

**Hinweis:**  
Für PlatformIO-Integration genügt das Kommando:  
```bash
pio run --target uploadfs --upload-port <IP>
```

---

## 🔒 Sicherheit

- Optional kann ein OTA-Passwort gesetzt werden
- Zugriff auf das Webinterface nur im lokalen Netz empfohlen

---

## 📦 Dateien & Struktur

| Pfad             | Inhalt                          |
|------------------|---------------------------------|
| `src/`           | Beispielprojekt mit OTA         |
| `platformio.ini` | PlatformIO-Konfiguration        |
| `README.md`      | Diese Anleitung                 |

---

## 💡 Tipps

- Vor OTA-Update sicherstellen, dass das Gerät im gleichen Netzwerk erreichbar ist
- OTA kann auch für SPIFFS/FS-Updates genutzt werden

---

**Viel Erfolg beim kabellosen Updaten deiner ESP-Projekte!** 🚀