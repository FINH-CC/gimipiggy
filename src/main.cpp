//
// Gimi Piggy, Copyright 2026.
//

#include <Arduino.h>

#include "piggy_graphics_lvgl.h"
#include "piggy_wifi_manager.h"
#include "piggy_html_fetch.h"
#include "piggy_printer.h"

void setup() {

  Serial.begin(115200);

  piggy_graphics_lvgl_setup();
  piggy_html_fetch_setup();
  piggy_wifi_manager_setup();
  piggy_printer_setup();
}

void loop() {
  piggy_wifi_manager_update();

  if (piggy_wifi_manager_is_connected() == true) {

    piggy_graphics_lvgl_update();
    piggy_html_fetch_update();
    piggy_printer_update();
  }
}