//
// Gimi Piggy, Copyright 2026.
//

#include <Arduino.h>

#include "wifi_manager.h"
#include "graphics_lvgl.h"

void setup() {

    Serial.begin(115200);

    graphics_lvgl_setup();
    wifi_manager_setup();
}

void loop() {
    wifi_manager_loop();
}