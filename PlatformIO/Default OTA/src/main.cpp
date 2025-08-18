#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define WebServer ESP8266WebServer
#elif defined(ESP32)
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#endif

#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <DNSServer.h>

// WiFi Konfiguration
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Access Point Konfiguration
#ifdef ESP8266
const char* ap_ssid = "ESP8266-OTA";
#elif defined(ESP32)
const char* ap_ssid = "ESP32-OTA";
#endif
const char* ap_password = "12345678";

// Web Server und Update Server
WebServer server(80);

// DNS Server für Captive Portal
DNSServer dnsServer;
const byte DNS_PORT = 53;

// Timeout für WiFi Verbindung (in Millisekunden)
const unsigned long wifiTimeout = 10000;

// Function declarations
void handleRoot();
void handleInfo();
void handleRestart();
void handleCaptivePortal();
void handleUpdate();
void handleUpdateUpload();
void handleAndroidCaptivePortal();

void setup() {
  Serial.begin(115200);
  Serial.println();
#ifdef ESP8266
  Serial.println("ESP8266 OTA Update Server");
#elif defined(ESP32)
  Serial.println("ESP32 OTA Update Server");
#endif
  
  // Dateisystem initialisieren
#ifdef ESP8266
  if (!LittleFS.begin()) {
    Serial.println("LittleFS Mount fehlgeschlagen");
  }
#elif defined(ESP32)
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS Mount fehlgeschlagen");
  }
#endif
  
  // Versuche WiFi Verbindung
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
#ifdef ESP8266
  WiFi.hostname("ESP8266-OTA");
#elif defined(ESP32)
  WiFi.setHostname("ESP32-OTA");
#endif
  
  Serial.print("Verbinde mit WiFi");
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < wifiTimeout) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    // WiFi erfolgreich verbunden
    Serial.println();
    Serial.println("WiFi verbunden!");
    Serial.print("IP Adresse: ");
    Serial.println(WiFi.localIP());
    Serial.print("Hostname: ");
#ifdef ESP8266
    Serial.println(WiFi.hostname());
#elif defined(ESP32)
    Serial.println(WiFi.getHostname());
#endif
  } else {
    // WiFi Verbindung fehlgeschlagen - starte Access Point
    Serial.println();
    Serial.println("WiFi Verbindung fehlgeschlagen - starte Access Point");
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);
    
    // Warte kurz bis AP gestartet ist
    delay(100);
    
    // Captive Portal DNS Server starten
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
    
    Serial.println("Access Point gestartet");
    Serial.print("AP SSID: ");
    Serial.println(ap_ssid);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
#ifdef ESP8266
    Serial.println("Captive Portal aktiv - alle DNS Anfragen werden zur ESP8266 Webseite weitergeleitet");
#elif defined(ESP32)
    Serial.println("Captive Portal aktiv - alle DNS Anfragen werden zur ESP32 Webseite weitergeleitet");
#endif
    Serial.println("Teste das Captive Portal mit HTTP-URLs (nicht HTTPS):");
    Serial.println("✅ FUNKTIONIERT: http://neverssl.com, http://bb.de");
    Serial.println("❌ FUNKTIONIERT NICHT: https://amazon.de, example.com (wird zu HTTPS)");
    Serial.println("💡 TIPP: Verwende 'neverssl.com' - diese Seite ist speziell für Captive Portals");
  }
  
  // Web Interface Routen
  server.on("/", handleRoot);
  server.on("/info", handleInfo);
  server.on("/restart", handleRestart);
  server.on("/update", HTTP_GET, handleUpdate);
  
#ifdef ESP8266
  server.on("/update", HTTP_POST, handleUpdateUpload, handleUpdateUpload);
#elif defined(ESP32)
  server.on("/update", HTTP_POST, [](){
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, handleUpdateUpload);
#endif
  
  // Android/iOS Captive Portal Detection Routen
  server.on("/generate_204", handleAndroidCaptivePortal);
  server.on("/gen_204", handleAndroidCaptivePortal);
  server.on("/ncsi.txt", handleAndroidCaptivePortal);
  server.on("/hotspot-detect.html", handleAndroidCaptivePortal);
  server.on("/connecttest.txt", handleAndroidCaptivePortal);
  server.on("/redirect", handleAndroidCaptivePortal);
  server.on("/success.txt", handleAndroidCaptivePortal);
  server.on("/canonical.html", handleAndroidCaptivePortal);
  server.on("/mobile/status.php", handleAndroidCaptivePortal);
  server.on("/kindle-wifi/wifiredirect.html", handleAndroidCaptivePortal);
  
  // Captive Portal - fange alle anderen Anfragen ab
  server.onNotFound(handleCaptivePortal);
  
  // ArduinoOTA einrichten (funktioniert nur bei WiFi Verbindung)
  if (WiFi.status() == WL_CONNECTED) {
#ifdef ESP8266
    ArduinoOTA.setHostname("ESP8266-OTA");
#elif defined(ESP32)
    ArduinoOTA.setHostname("ESP32-OTA");
#endif
    ArduinoOTA.setPassword("admin");
    
    ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {
        type = "filesystem";
      }
      Serial.println("Start updating " + type);
    });
    
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });
    
    ArduinoOTA.begin();
    Serial.println("ArduinoOTA bereit");
  }
  
  server.begin();
  Serial.println("HTTP Server gestartet");
  Serial.println("Gehe zu:");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("http://" + WiFi.localIP().toString() + "/");
    Serial.println("http://" + WiFi.localIP().toString() + "/update");
  } else {
    Serial.println("http://" + WiFi.softAPIP().toString() + "/");
    Serial.println("http://" + WiFi.softAPIP().toString() + "/update");
  }
}

void loop() {
  server.handleClient();
  
  // DNS Server für Captive Portal verarbeiten
  if (WiFi.getMode() == WIFI_AP) {
    dnsServer.processNextRequest();
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    ArduinoOTA.handle();
  }
  
  // Debug: Zeige DNS Aktivität
  static unsigned long lastDNSActivity = 0;
  static int dnsRequestCount = 0;
  
  if (millis() - lastDNSActivity > 10000) { // Alle 10 Sekunden
    if (dnsRequestCount > 0) {
      Serial.println("DNS Server aktiv - " + String(dnsRequestCount) + " Anfragen in den letzten 10s");
      dnsRequestCount = 0;
    }
    lastDNSActivity = millis();
  }
}

void handleRoot() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
)";
#ifdef ESP8266
  html += "<title>ESP8266 OTA Update</title>";
#elif defined(ESP32)
  html += "<title>ESP32 OTA Update</title>";
#endif
  html += R"(
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: Arial; margin: 20px; background: #f0f0f0; }
    .container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; }
    h1 { color: #333; text-align: center; }
    .button { display: inline-block; padding: 10px 20px; margin: 10px 5px; background: #007cba; color: white; text-decoration: none; border-radius: 5px; }
    .button:hover { background: #005a87; }
    .info { background: #e7f3ff; padding: 15px; border-radius: 5px; margin: 10px 0; }
    .warning { background: #fff3cd; border: 1px solid #ffeaa7; padding: 10px; border-radius: 5px; margin: 10px 0; }
  </style>
</head>
<body>
  <div class='container'>
)";
#ifdef ESP8266
    html += "<h1>ESP8266 OTA Update Server</h1>";
#elif defined(ESP32)
    html += "<h1>ESP32 OTA Update Server</h1>";
#endif
    html += R"(
    
    <div class='info'>
      <h3>Verfügbare Funktionen:</h3>
      <a href='/update' class='button'>📁 Firmware Update (Browser)</a>
      <a href='/info' class='button'>ℹ️ System Info</a>
      <a href='/restart' class='button'>🔄 Neustart</a>
    </div>
    
    <div class='warning'>
      <h3>OTA Update Methoden:</h3>
      <p><strong>1. Browser Upload:</strong> Klicke auf "Firmware Update" um .bin Dateien hochzuladen</p>
)";
  if (WiFi.status() == WL_CONNECTED) {
#ifdef ESP8266
    html += R"(
      <p><strong>2. Arduino IDE OTA:</strong> Verwende "ESP8266-OTA" als Port (Passwort: admin)</p>
)";
#elif defined(ESP32)
    html += R"(
      <p><strong>2. Arduino IDE OTA:</strong> Verwende "ESP32-OTA" als Port (Passwort: admin)</p>
)";
#endif
    html += R"(
      <p><strong>3. PlatformIO OTA:</strong> Konfiguriere upload_port und upload_flags in platformio.ini</p>
)";
  } else {
    html += R"(
      <p><strong>Hinweis:</strong> Arduino IDE OTA ist nur im WiFi-Modus verfügbar</p>
)";
  }

  html += R"(
    </div>
    
    <div class='info'>
      <h3>Update Arten:</h3>
      <p><strong>Firmware (.bin):</strong> Programmcode Update</p>
      <p><strong>Filesystem (.bin):</strong> LittleFS Daten Update</p>
    </div>
    
  </div>
</body>
</html>
)";
  server.send(200, "text/html", html);
}

void handleInfo() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
  <title>System Information</title>
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: Arial; margin: 20px; background: #f0f0f0; }
    .container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; }
    h1 { color: #333; text-align: center; }
    .info-table { width: 100%; border-collapse: collapse; }
    .info-table td { padding: 8px; border-bottom: 1px solid #ddd; }
    .info-table td:first-child { font-weight: bold; background: #f8f9fa; }
    .button { display: inline-block; padding: 10px 20px; margin: 10px 5px; background: #007cba; color: white; text-decoration: none; border-radius: 5px; }
  </style>
</head>
<body>
  <div class='container'>
)";
#ifdef ESP8266
    html += "<h1>ESP8266 System Information</h1>";
#elif defined(ESP32)
    html += "<h1>ESP32 System Information</h1>";
#endif
    html += "<table class='info-table'>";

#ifdef ESP8266
  html += "<tr><td>Chip ID</td><td>" + String(ESP.getChipId(), HEX) + "</td></tr>";
  html += "<tr><td>Flash Chip ID</td><td>" + String(ESP.getFlashChipId(), HEX) + "</td></tr>";
  html += "<tr><td>Flash Größe</td><td>" + String(ESP.getFlashChipSize() / 1024) + " KB</td></tr>";
  html += "<tr><td>Flash Geschwindigkeit</td><td>" + String(ESP.getFlashChipSpeed() / 1000000) + " MHz</td></tr>";
  html += "<tr><td>Freier Heap</td><td>" + String(ESP.getFreeHeap()) + " Bytes</td></tr>";
  html += "<tr><td>Sketch Größe</td><td>" + String(ESP.getSketchSize()) + " Bytes</td></tr>";
  html += "<tr><td>Freier Sketch Platz</td><td>" + String(ESP.getFreeSketchSpace()) + " Bytes</td></tr>";
  html += "<tr><td>CPU Frequenz</td><td>" + String(ESP.getCpuFreqMHz()) + " MHz</td></tr>";
  html += "<tr><td>Reset Grund</td><td>" + ESP.getResetReason() + "</td></tr>";
#elif defined(ESP32)
  html += "<tr><td>Chip Model</td><td>" + String(ESP.getChipModel()) + "</td></tr>";
  html += "<tr><td>Chip Revision</td><td>" + String(ESP.getChipRevision()) + "</td></tr>";
  html += "<tr><td>CPU Cores</td><td>" + String(ESP.getChipCores()) + "</td></tr>";
  html += "<tr><td>CPU Frequenz</td><td>" + String(ESP.getCpuFreqMHz()) + " MHz</td></tr>";
  html += "<tr><td>Flash Größe</td><td>" + String(ESP.getFlashChipSize() / 1024) + " KB</td></tr>";
  html += "<tr><td>Flash Geschwindigkeit</td><td>" + String(ESP.getFlashChipSpeed() / 1000000) + " MHz</td></tr>";
  html += "<tr><td>Freier Heap</td><td>" + String(ESP.getFreeHeap()) + " Bytes</td></tr>";
  html += "<tr><td>Sketch Größe</td><td>" + String(ESP.getSketchSize()) + " Bytes</td></tr>";
  html += "<tr><td>Freier Sketch Platz</td><td>" + String(ESP.getFreeSketchSpace()) + " Bytes</td></tr>";
  html += "<tr><td>SDK Version</td><td>" + String(ESP.getSdkVersion()) + "</td></tr>";
#endif
  
  if (WiFi.status() == WL_CONNECTED) {
    html += "<tr><td>WiFi Status</td><td>Verbunden</td></tr>";
    html += "<tr><td>SSID</td><td>" + WiFi.SSID() + "</td></tr>";
    html += "<tr><td>IP Adresse</td><td>" + WiFi.localIP().toString() + "</td></tr>";
    html += "<tr><td>MAC Adresse</td><td>" + WiFi.macAddress() + "</td></tr>";
    html += "<tr><td>Signal Stärke</td><td>" + String(WiFi.RSSI()) + " dBm</td></tr>";
  } else {
    html += "<tr><td>WiFi Status</td><td>Access Point Modus</td></tr>";
    html += "<tr><td>AP SSID</td><td>" + String(ap_ssid) + "</td></tr>";
    html += "<tr><td>AP IP</td><td>" + WiFi.softAPIP().toString() + "</td></tr>";
  }
  
  html += R"(
    </table>
    <br>
    <a href='/' class='button'>🏠 Zurück</a>
  </div>
</body>
</html>
)";
  server.send(200, "text/html", html);
}

void handleRestart() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Neustart</title>
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: Arial; margin: 20px; background: #f0f0f0; text-align: center; }
    .container { max-width: 400px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; }
  </style>
  <script>
    setTimeout(function() {
      window.location.href = '/';
    }, 5000);
  </script>
</head>
<body>
  <div class='container'>
    <h1>🔄 Neustart</h1>
)";
#ifdef ESP8266
    html += "<p>ESP8266 wird neu gestartet...</p>";
#elif defined(ESP32)
    html += "<p>ESP32 wird neu gestartet...</p>";
#endif
    html += R"(
    <p>Du wirst in 5 Sekunden zur Hauptseite weitergeleitet.</p>
  </div>
</body>
</html>
)";
  server.send(200, "text/html", html);
  delay(1000);
  ESP.restart();
}

void handleAndroidCaptivePortal() {
  String uri = server.uri();
  Serial.println("Android Captive Portal Request: " + uri);
  
  // Für generate_204 - Standard Android Connectivity Check
  if (uri.indexOf("generate_204") >= 0) {
    // Gebe bewusst NICHT Status 204 zurück um Captive Portal zu triggern
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.send(200, "text/html", "<!DOCTYPE html><html><head><title>ESP8266</title></head><body><h1>ESP8266 Setup</h1><p><a href='http://" + WiFi.softAPIP().toString() + "'>Setup öffnen</a></p></body></html>");
    return;
  }
  
  // Für alle anderen Captive Portal URLs
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache"); 
  server.sendHeader("Expires", "-1");
  server.send(200, "text/html", "<!DOCTYPE html><html><head><title>ESP8266</title></head><body><h1>ESP8266 Setup</h1><p><a href='http://" + WiFi.softAPIP().toString() + "'>Setup öffnen</a></p></body></html>");
}

void handleCaptivePortal() {
  if (WiFi.getMode() == WIFI_AP) {
    String uri = server.uri();
    String host = server.hostHeader();
    String userAgent = server.header("User-Agent");
    
    Serial.println("=== CAPTIVE PORTAL REQUEST ===");
    Serial.println("URI: " + uri);
    Serial.println("Host: " + host);
    Serial.println("Method: " + String(server.method()));
    Serial.println("User-Agent: " + userAgent);
    Serial.println("Headers: " + String(server.headers()));
    for(int i = 0; i < server.headers(); i++) {
      Serial.println("  " + server.headerName(i) + ": " + server.header(i));
    }
    Serial.println("===============================");
    
    // Für bekannte Captive Portal Test URLs
    if (uri.indexOf("generate_204") >= 0 || 
        uri.indexOf("gen_204") >= 0 || 
        uri.indexOf("connecttest") >= 0 ||
        uri.indexOf("msftconnecttest") >= 0 ||
        uri.indexOf("ncsi.txt") >= 0 ||
        uri.indexOf("hotspot-detect") >= 0 ||
        uri.indexOf("success.txt") >= 0) {
      
      Serial.println("-> Handling as Android Captive Portal");
      handleAndroidCaptivePortal();
      return;
    }
    
    // Prüfe ob die Anfrage direkt an die IP-Adresse geht
    if (host == WiFi.softAPIP().toString() || host == "192.168.4.1") {
      Serial.println("-> Direct IP request - showing main page");
      handleRoot();
      return;
    }
    
    String redirectURL = "http://" + WiFi.softAPIP().toString() + "/";
    
    Serial.println("-> SUCCESS! Showing captive portal page for: " + host + uri);
    
    // Status für bekannte URLs
    String urlStatus = "";
    if (host.indexOf("example.com") >= 0) {
      urlStatus = "⚠️ example.com wird oft automatisch zu HTTPS umgeleitet";
    } else if (host.indexOf("test.de") >= 0) {
      urlStatus = "⚠️ test.de kann HTTPS-Redirect haben";
    } else if (host.indexOf("neverssl.com") >= 0) {
      urlStatus = "✅ neverssl.com ist perfekt für Captive Portals";
    } else if (host.indexOf("bb.de") >= 0) {
      urlStatus = "✅ bb.de funktioniert gut mit HTTP";
    } else {
      urlStatus = "🔍 Unbekannte URL - sollte funktionieren wenn HTTP verwendet wird";
    }
    
    // Für alle anderen Anfragen - zeige immer eine Captive Portal Seite
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    
    String captiveHTML = R"(<!DOCTYPE html>
<html>
<head>
  <title>🎯 Captive Portal Erfolgreich!</title>
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { 
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
      text-align: center; 
      margin: 0; 
      padding: 20px; 
      background: linear-gradient(135deg, #28a745 0%, #20c997 100%);
      color: white;
      min-height: 100vh;
      box-sizing: border-box;
    }
    .container { 
      max-width: 600px; 
      margin: 20px auto; 
      background: rgba(255,255,255,0.95); 
      color: #333;
      padding: 30px; 
      border-radius: 15px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.3);
    }
    .success-icon {
      font-size: 80px;
      color: #28a745;
      margin-bottom: 20px;
      animation: bounce 2s infinite;
    }
    @keyframes bounce {
      0%, 20%, 50%, 80%, 100% { transform: translateY(0); }
      40% { transform: translateY(-10px); }
      60% { transform: translateY(-5px); }
    }
    .button {
      display: inline-block;
      padding: 15px 30px;
      background: #007cba;
      color: white;
      text-decoration: none;
      border-radius: 8px;
      margin: 15px 10px;
      font-size: 18px;
      transition: all 0.3s ease;
      box-shadow: 0 4px 15px rgba(0,124,186,0.3);
    }
    .button:hover {
      background: #005a87;
      transform: translateY(-2px);
      box-shadow: 0 6px 20px rgba(0,124,186,0.4);
    }
    .info {
      background: #e8f5e8;
      padding: 20px;
      border-radius: 10px;
      margin: 20px 0;
      border-left: 4px solid #28a745;
    }
    .requested-site {
      color: #666;
      font-style: italic;
      margin: 15px 0;
      padding: 15px;
      background: #f8f9fa;
      border-radius: 8px;
      word-break: break-all;
      border: 2px solid #28a745;
    }
    .url-status {
      background: #fff3e0;
      padding: 15px;
      border-radius: 10px;
      margin: 20px 0;
      font-size: 14px;
      border-left: 4px solid #ff9800;
    }
    .test-urls {
      background: #e3f2fd;
      padding: 20px;
      border-radius: 10px;
      margin: 20px 0;
      text-align: left;
      border-left: 4px solid #2196f3;
    }
  </style>
</head>
<body>
  <div class='container'>
    <div class='success-icon'>🎯</div>
    <h1>Captive Portal funktioniert perfekt!</h1>
    <p><strong>Glückwunsch!</strong> Das ESP8266 Captive Portal hat deine Anfrage erfolgreich abgefangen.</p>
    
    <div class='requested-site'>
      <strong>Du wolltest besuchen:</strong><br>
      )" + host + uri + R"(
    </div>
    
    <div class='url-status'>
      <strong>Status dieser URL:</strong><br>
      )" + urlStatus + R"(
    </div>
    
    <div class='info'>
      <h3>🎯 System Status:</h3>
      <p>✅ <strong>DNS Server:</strong> Aktiv und funktioniert</p>
      <p>✅ <strong>HTTP Server:</strong> Läuft auf )" + WiFi.softAPIP().toString() + R"(</p>
      <p>✅ <strong>Captive Portal:</strong> Erfolgreich aktiviert</p>
    </div>
    
    <div class='test-urls'>
      <h4>📋 Weitere Test-URLs die funktionieren:</h4>
      <p>✅ <strong>neverssl.com</strong> - Speziell für Captive Portals</p>
      <p>✅ <strong>bb.de</strong> - Deutsche Seite mit HTTP</p>
      <p>✅ <strong>httpforever.com</strong> - Immer HTTP</p>
      <p>❌ <strong>example.com, test.de</strong> - Werden oft zu HTTPS umgeleitet</p>
    </div>
    
    <a href=')" + redirectURL + R"(' class='button'>
      🔧 ESP8266 OTA Setup
    </a>
    
    <a href=')" + redirectURL + R"(info' class='button' style='background: #28a745;'>
      ℹ️ System Information
    </a>
    
    <p style='margin-top: 30px; font-size: 14px; color: #666;'>
      <strong>Perfekt!</strong> Dein Captive Portal funktioniert einwandfrei.
    </p>
  </div>

  <script>
    console.log('🎯 CAPTIVE PORTAL SUCCESS!');
    console.log('Original request: )" + host + uri + R"(');
    console.log('ESP8266 IP: )" + WiFi.softAPIP().toString() + R"(');
    console.log('This means DNS and HTTP capture are working perfectly!');
    
    // Automatische Weiterleitung nach 20 Sekunden
    let countdown = 20;
    const timer = setInterval(function() {
      countdown--;
      if (countdown <= 0) {
        console.log('Auto-redirecting to ESP8266 setup...');
        window.location.href = ')" + redirectURL + R"(';
        clearInterval(timer);
      }
    }, 1000);
  </script>
</body>
</html>)";
    
    server.send(200, "text/html", captiveHTML);
    
  } else {
    server.send(404, "text/plain", "Seite nicht gefunden");
  }
}

void handleUpdate() {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
)";
#ifdef ESP8266
  html += "<title>ESP8266 OTA Firmware Update</title>";
#elif defined(ESP32)
  html += "<title>ESP32 OTA Firmware Update</title>";
#endif
  
  html += R"(
  <meta charset='utf-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { 
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; 
      margin: 0; 
      padding: 20px; 
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); 
      min-height: 100vh;
    }
    .container { 
      max-width: 600px; 
      margin: 0 auto; 
      background: white; 
      padding: 30px; 
      border-radius: 15px; 
      box-shadow: 0 10px 30px rgba(0,0,0,0.3);
    }
    h1 { 
      color: #333; 
      text-align: center; 
      margin-bottom: 30px;
      font-size: 2.5em;
    }
    .upload-area {
      border: 3px dashed #007cba;
      border-radius: 10px;
      padding: 40px;
      text-align: center;
      background: #f8f9ff;
      margin: 20px 0;
      transition: all 0.3s ease;
    }
    .upload-area:hover {
      border-color: #005a87;
      background: #e7f3ff;
    }
    .upload-area.dragover {
      border-color: #28a745;
      background: #d4edda;
    }
    .file-input {
      display: none;
    }
    .upload-btn {
      display: inline-block;
      padding: 15px 30px;
      background: #007cba;
      color: white;
      border: none;
      border-radius: 8px;
      font-size: 16px;
      cursor: pointer;
      transition: background 0.3s ease;
    }
    .upload-btn:hover {
      background: #005a87;
    }
    .progress-container {
      margin: 20px 0;
      display: none;
    }
    .progress-bar {
      width: 100%;
      height: 20px;
      background: #f0f0f0;
      border-radius: 10px;
      overflow: hidden;
      box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);
    }
    .progress-fill {
      height: 100%;
      background: linear-gradient(90deg, #28a745, #20c997);
      width: 0%;
      transition: width 0.3s ease;
      border-radius: 10px;
    }
    .progress-text {
      text-align: center;
      margin-top: 10px;
      font-weight: bold;
      color: #333;
    }
    .info-box {
      background: #e7f3ff;
      border-left: 4px solid #007cba;
      padding: 15px;
      margin: 20px 0;
      border-radius: 5px;
    }
    .warning-box {
      background: #fff3cd;
      border-left: 4px solid #ffc107;
      padding: 15px;
      margin: 20px 0;
      border-radius: 5px;
    }
    .button {
      display: inline-block;
      padding: 10px 20px;
      margin: 10px 5px;
      background: #6c757d;
      color: white;
      text-decoration: none;
      border-radius: 5px;
      transition: background 0.3s ease;
    }
    .button:hover {
      background: #545b62;
    }
    .file-info {
      margin: 15px 0;
      padding: 10px;
      background: #f8f9fa;
      border-radius: 5px;
      display: none;
    }
    .status-message {
      margin: 15px 0;
      padding: 15px;
      border-radius: 5px;
      display: none;
    }
    .success { background: #d4edda; border: 1px solid #c3e6cb; color: #155724; }
    .error { background: #f8d7da; border: 1px solid #f5c6cb; color: #721c24; }
  </style>
</head>
<body>
  <div class='container'>
    <h1>🚀 Firmware Update</h1>
    
    <div class='info-box'>
      <h3>📋 Anleitung:</h3>
      <p>1. Wähle eine .bin Datei aus (Firmware oder Filesystem)</p>
      <p>2. Die Datei wird automatisch hochgeladen und installiert</p>
      <p>3. Der ESP8266 startet nach erfolgreichem Update automatisch neu</p>
    </div>
    
    <form id='uploadForm' enctype='multipart/form-data'>
      <div class='upload-area' id='uploadArea'>
        <div style='font-size: 48px; margin-bottom: 20px;'>📁</div>
        <p style='font-size: 18px; margin: 10px 0;'>Klicke hier oder ziehe eine .bin Datei hierher</p>
        <button type='button' class='upload-btn' onclick='document.getElementById("fileInput").click()'>
          Datei auswählen
        </button>
        <input type='file' id='fileInput' name='update' accept='.bin' class='file-input'>
      </div>
      
      <div class='file-info' id='fileInfo'></div>
      
      <div class='progress-container' id='progressContainer'>
        <div class='progress-bar'>
          <div class='progress-fill' id='progressFill'></div>
        </div>
        <div class='progress-text' id='progressText'>Upload wird vorbereitet...</div>
      </div>
      
      <div class='status-message' id='statusMessage'></div>
    </form>
    
    <div class='warning-box'>
      <h3>⚠️ Wichtige Hinweise:</h3>
      <p>• Stelle sicher, dass die .bin Datei für ESP8266 kompiliert wurde</p>
      <p>• Unterbreche den Update-Vorgang nicht</p>
      <p>• Bei Problemen: Neustart über die Hardware Reset-Taste</p>
    </div>
    
    <div style='text-align: center; margin-top: 30px;'>
      <a href='/' class='button'>🏠 Zurück zur Hauptseite</a>
    </div>
  </div>

  <script>
    const uploadArea = document.getElementById('uploadArea');
    const fileInput = document.getElementById('fileInput');
    const fileInfo = document.getElementById('fileInfo');
    const progressContainer = document.getElementById('progressContainer');
    const progressFill = document.getElementById('progressFill');
    const progressText = document.getElementById('progressText');
    const statusMessage = document.getElementById('statusMessage');

    // Drag & Drop Events
    uploadArea.addEventListener('dragover', (e) => {
      e.preventDefault();
      uploadArea.classList.add('dragover');
    });

    uploadArea.addEventListener('dragleave', () => {
      uploadArea.classList.remove('dragover');
    });

    uploadArea.addEventListener('drop', (e) => {
      e.preventDefault();
      uploadArea.classList.remove('dragover');
      const files = e.dataTransfer.files;
      if (files.length > 0) {
        handleFile(files[0]);
      }
    });

    // File Input Change
    fileInput.addEventListener('change', (e) => {
      if (e.target.files.length > 0) {
        handleFile(e.target.files[0]);
      }
    });

    function handleFile(file) {
      if (!file.name.endsWith('.bin')) {
        showStatus('Bitte wähle eine .bin Datei aus!', 'error');
        return;
      }

      // File Info anzeigen
      fileInfo.innerHTML = `
        <strong>Datei:</strong> ${file.name}<br>
        <strong>Größe:</strong> ${(file.size / 1024).toFixed(2)} KB<br>
        <strong>Typ:</strong> ${file.name.includes('spiffs') || file.name.includes('littlefs') || file.name.includes('filesystem') ? 'Filesystem' : 'Firmware'}
      `;
      fileInfo.style.display = 'block';
      
      // Upload starten
      uploadFile(file);
    }

    function uploadFile(file) {
      const formData = new FormData();
      formData.append('update', file);

      const xhr = new XMLHttpRequest();

      // Progress Event
      xhr.upload.addEventListener('progress', (e) => {
        if (e.lengthComputable) {
          const percentComplete = (e.loaded / e.total) * 100;
          progressFill.style.width = percentComplete + '%';
          progressText.textContent = `Upload: ${percentComplete.toFixed(1)}%`;
        }
      });

      // Load Event
      xhr.addEventListener('load', () => {
        if (xhr.status === 200) {
          progressFill.style.width = '100%';
          progressText.textContent = 'Update erfolgreich! ESP8266 startet neu...';
          showStatus('✅ Update erfolgreich abgeschlossen! Der ESP8266 startet jetzt neu.', 'success');
          
          // Nach 3 Sekunden zur Hauptseite weiterleiten
          setTimeout(() => {
            window.location.href = '/';
          }, 3000);
        } else {
          showStatus('❌ Update fehlgeschlagen: ' + xhr.responseText, 'error');
        }
      });

      // Error Event
      xhr.addEventListener('error', () => {
        showStatus('❌ Verbindungsfehler beim Upload', 'error');
      });

      // Upload starten
      progressContainer.style.display = 'block';
      progressText.textContent = 'Upload wird gestartet...';
      
      xhr.open('POST', '/update');
      xhr.send(formData);
    }

    function showStatus(message, type) {
      statusMessage.textContent = message;
      statusMessage.className = 'status-message ' + type;
      statusMessage.style.display = 'block';
    }
  </script>
</body>
</html>
)";
  
  server.send(200, "text/html", html);
}

void handleUpdateUpload() {
  HTTPUpload& upload = server.upload();
  
#ifdef ESP8266
  static bool updateStarted = false;

  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Update Start: %s\n", upload.filename.c_str());
    updateStarted = false;
    
    // Bestimme Update-Typ basierend auf Dateiname
    bool isFilesystem = (upload.filename.indexOf("spiffs") >= 0 || 
                        upload.filename.indexOf("littlefs") >= 0 || 
                        upload.filename.indexOf("filesystem") >= 0);
    
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    
    if (isFilesystem) {
      // Filesystem Update
      if (!Update.begin(FS_end - FS_start, U_FS)) {
        Update.printError(Serial);
        return;
      }
    } else {
      // Firmware Update
      if (!Update.begin(maxSketchSpace, U_FLASH)) {
        Update.printError(Serial);
        return;
      }
    }
    updateStarted = true;
    
  } else if (upload.status == UPLOAD_FILE_WRITE && updateStarted) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
    
  } else if (upload.status == UPLOAD_FILE_END && updateStarted) {
    if (Update.end(true)) {
      Serial.printf("Update Success: %u bytes\n", upload.totalSize);
      server.send(200, "text/plain", "Update erfolgreich");
      delay(1000);
      ESP.restart();
    } else {
      Update.printError(Serial);
      server.send(500, "text/plain", "Update fehlgeschlagen");
    }
    
  } else if (upload.status == UPLOAD_FILE_ABORTED) {
    Update.end();
    Serial.println("Update abgebrochen");
    server.send(500, "text/plain", "Update abgebrochen");
  }

#elif defined(ESP32)
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("Update Start: %s\n", upload.filename.c_str());
    
    // Bestimme Update-Typ basierend auf Dateiname
    if (upload.filename.indexOf("spiffs") >= 0 || 
        upload.filename.indexOf("littlefs") >= 0 ||
        upload.filename.indexOf("filesystem") >= 0) {
      // Für ESP32: Verwende U_SPIFFS für Filesystem Updates (funktioniert auch mit LittleFS)
      if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_SPIFFS)) {
        Update.printError(Serial);
      }
    } else {
      // Firmware Update
      if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH)) {
        Update.printError(Serial);
      }
    }
    
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
    
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      Serial.printf("Update Success: %u bytes\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
  }
#endif
}
