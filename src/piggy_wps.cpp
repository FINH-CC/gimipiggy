//
// Gimi Piggy, Copyright 2026.
//

#include <WiFi.h>
#include <esp_wps.h>

#include "piggy_wps.h"

#define WPS_BUTTON_PIN 0 // Using onboard BOOT button on many ESP32 devkits
#define ESP_WPS_MODE WPS_TYPE_PBC

static esp_wps_config_t config;

void piggy_wps_init_config() {
//    config.crypto_funcs = &g_wifi_default_wps_crypto_funcs;
    config.wps_type = ESP_WPS_MODE;
}

void piggy_wifi_event(WiFiEvent_t event, arduino_event_info_t info) {
    switch (event) {
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WPS Successful, connecting...");
            esp_wifi_wps_disable();
            WiFi.begin(); // Reconnect with new credentials
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WPS Failed/Timeout, retrying...");
            esp_wifi_wps_disable();
            esp_wifi_wps_enable(&config);
            esp_wifi_wps_start(0);
            break;
        default: break;
    }
}

void piggy_wps_setup() {
    pinMode(WPS_BUTTON_PIN, INPUT_PULLUP);
    WiFi.onEvent(piggy_wifi_event);
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(); // Try to connect with stored credentials
}

void piggy_wps_update() {
    if (digitalRead(WPS_BUTTON_PIN) == LOW) {
        delay(50); // Debounce
        if (digitalRead(WPS_BUTTON_PIN) == LOW) {
            Serial.println("WPS Button Pressed - Starting WPS");
            piggy_wps_init_config();
            esp_wifi_wps_enable(&config);
            esp_wifi_wps_start(0);
            while(digitalRead(WPS_BUTTON_PIN) == LOW); // Wait for release
        }
    }
}
