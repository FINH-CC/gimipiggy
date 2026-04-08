//
// Gimi Piggy, Copyright 2026.
//

#include <Arduino.h>

#include "graphics_lvgl.h"
#include "wifi_manager.h"
#include "html_fetch.h"

void setup() {

  Serial.begin(115200);

  graphics_lvgl_setup();
  html_fetch_setup();
  wifi_manager_setup();
}

void loop() {
  wifi_manager_update();

  if (wifi_manager_is_connected() == true) {

    graphics_lvgl_update();
    html_fetch_update();
  }
}