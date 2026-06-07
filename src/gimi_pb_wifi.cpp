//
// Copyright 2026.
//

#include <Arduino.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

#include "gimi_pb_wifi_html.h"
#include "gimi_pb_html_fetch.h"
#include "gimi_pb_wifi.h"

#define WIFI_CONNECTION_ATTEMPTS 30
#define WIFI_CONNECTION_INCREASE_DELAY_AFTER_ATTEMPTS 15
#define WIFI_CONNECTION_ATTEMPT_DELAY_SHORT 1000
#define WIFI_CONNECTION_ATTEMPT_DELAY_LONG 5000

// DNS server for captive portal
DNSServer dnsServer;
WebServer server(80);
Preferences preferences;

// Configuration variables
String ssid = "";
String password = "";
bool configMode = false;

// AP configuration
const char* apSSID = "gimi";
const char* apPassword = "gimi";
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

}

bool gimi_pb_wifi_manager_setup() {

  wifi_is_connected = false;

  // Initialize preferences
  preferences.begin("wifi-config", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");

  Serial.println("\n\nGimi PB WiFi Configuration Portal");
  Serial.println("================================");

  // Try to connect to saved WiFi
  if (ssid.length() > 0) {
    Serial.println("Attempting to connect to saved WiFi: " + ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECTION_ATTEMPTS) {
    
    if (attempts <= WIFI_CONNECTION_INCREASE_DELAY_AFTER_ATTEMPTS)
     delay(WIFI_CONNECTION_ATTEMPT_DELAY_SHORT);
    else
     delay(WIFI_CONNECTION_ATTEMPT_DELAY_LONG);

    Serial.print(".");
    attempts++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
    } else {
      Serial.println("Failed to connect to saved WiFi");
      return false;
    }
  }

  // Start configuration mode
  startConfigMode();
  return true;
}

bool gimi_pb_wifi_manager_is_config_mode() {

  return configMode;
}

void gimi_pb_wifi_manager_reconnect() {
  
  if (configMode) {
    dnsServer.processNextRequest();
    server.handleClient();

  } else {
    // Normal operation mode
    if (WiFi.status() == WL_CONNECTED) {
      wifi_is_connected = true;
    } else {
      wifi_is_connected = false;
      // Lost connection, try to reconnect
      Serial.println("WiFi disconnected, attempting to reconnect...");
      WiFi.reconnect();
      delay(5000);

      if (WiFi.status() == WL_CONNECTED)
        wifi_is_connected = true;
    }
  }
    
  delay(10);
}

bool gimi_pb_wifi_manager_is_connected() {

  return wifi_is_connected;
}

void gimi_pb_wifi_manager_disconnect() {
  
  Serial.println("WiFi being disconnected.");
  WiFi.disconnect(false, false);
  wifi_is_connected = false;
}

void gimi_pb_wifi_manager_restart_wifi_setup() {

    Serial.println("Clearing WiFi credentials\n");

    // Save to preferences
    preferences.putString("ssid", "");
    preferences.putString("password", "");

    delay(2000);
    Serial.println("Restarting ESP32...");
    ESP.restart();
}

