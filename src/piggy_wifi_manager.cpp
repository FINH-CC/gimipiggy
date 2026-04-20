//
// Gimi Piggy, Copyright 2026.
//

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

#include "piggy_wifi_manager_html.h"
#include "piggy_wifi_manager.h"
#include "piggy_html_fetch.h"
#include "piggy_graphics_lvgl.h"

// Built-in LED pin (active-low on most ESP32 boards)
#define LED_PIN 2

// DNS server for captive portal
DNSServer dnsServer;
WebServer server(80);
Preferences preferences;

// Configuration variables
String ssid = "";
String password = "";
bool configMode = false;

// AP configuration
const char* apSSID = "gimipiggy";
const char* apPassword = "gimipiggy";
const byte DNS_PORT = 53;

bool wifi_is_connected;

void handleRoot() {

  server.send(200, "text/html", htmlPage);
}

void handleScan() {

  Serial.println("Scanning for networks...");
  int n = WiFi.scanNetworks();
    
  String json = "{\"networks\":[";
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"encryption\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "true" : "false");
    json += "}";
  }
  json += "]}";
    
  WiFi.scanDelete();
  server.send(200, "application/json", json);
}

void handleSave() {

  if (server.hasArg("ssid")) {
    ssid = server.arg("ssid");
    password = server.hasArg("password") ? server.arg("password") : "";

    Serial.println("\nReceived WiFi credentials:");
    Serial.println("SSID: " + ssid);
    Serial.println("Password: " + String(password.length() > 0 ? "********" : "(none)"));

    // Save to preferences
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);

    server.send(200, "text/html", successPage);

    delay(2000);
    Serial.println("Restarting ESP32...");
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Missing SSID parameter");
  }
}

void startConfigMode() {
    
  configMode = true;
  Serial.println("\nStarting Configuration Mode");
  Serial.println("===========================");

  // Start Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);

  Serial.print("AP SSID: ");
  Serial.println(apSSID);
  Serial.print("AP Password: ");
  Serial.println(apPassword);
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Start DNS server for captive portal
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Setup web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/scan", HTTP_GET, handleScan);
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound(handleRoot); // Redirect all requests to root for captive portal

  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Connect to the AP and navigate to http://192.168.4.1");

  // Blink LED to indicate config mode
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
  }
}

void piggy_wifi_manager_setup() {

  wifi_is_connected = false;

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // LED OFF (active-low)

  // Initialize preferences
  preferences.begin("wifi-config", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");

  Serial.println("\n\nGimi Piggy WiFi Configuration Portal");
  Serial.println("================================");

  // Try to connect to saved WiFi
  if (ssid.length() > 0) {
    Serial.println("Attempting to connect to saved WiFi: " + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

  // Blink LED while connecting
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
    Serial.print(".");
    attempts++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW); // LED ON - connected
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return;
    } else {
      Serial.println("Failed to connect to saved WiFi");
    }
  }

  // Start configuration mode
  startConfigMode();
}

void piggy_wifi_manager_update() {
  
  if (configMode) {
    dnsServer.processNextRequest();
    server.handleClient();

    // Slow blink LED in config mode
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 2000) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastBlink = millis();
    }
  } else {
    // Normal operation mode - LED steady on if connected
    if (WiFi.status() == WL_CONNECTED) {
      wifi_is_connected = true;
      digitalWrite(LED_PIN, LOW); // LED ON
    } else {
      wifi_is_connected = false;
      // Lost connection, try to reconnect
      digitalWrite(LED_PIN, HIGH); // LED OFF
      Serial.println("WiFi disconnected, attempting to reconnect...");
      WiFi.reconnect();
      delay(5000);
    }
  }
    
  delay(10);
}

bool piggy_wifi_manager_is_connected() {

  return wifi_is_connected;
}
