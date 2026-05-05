//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_wifi.h"
#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_buttons.h"

void setup() {

  Serial.begin(115200);

  Serial.println("GIMI PG STARTING");

  gimi_pb_wifi_manager_setup();
  gimi_pb_bin_file_setup();
  gimi_pb_printer_setup();
  gimi_pb_buttons_setup();
}

void loop() {
  gimi_pb_wifi_manager_update();

  if (gimi_pb_wifi_manager_is_connected() == true) {

    if (gimi_pb_button_0_debounced() == true) {

      gimi_pb_bin_file_update();
//    gimi_pb_printer_print_base64();
      gimi_pb_printer_print_binary();

//      gimi_pb_wifi_manager_restart_wifi_setup();
    }
  }

}