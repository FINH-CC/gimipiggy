//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_wifi.h"
#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_buttons.h"
#include "gimi_pb_leds.h"
#include "gimi_pb_server.h"

void setup() {

  Serial.begin(115200);

  Serial.println("GIMI PG STARTING");

  gimi_pb_wifi_manager_setup();
  gimi_pb_bin_file_setup();
  gimi_pb_printer_setup();
  gimi_pb_buttons_setup();
  gimi_pb_leds_setup();
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






#define buttonPin  GPIO_NUM_35

void not_setup() {

  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP); // pull-down resistor

  esp_sleep_enable_ext0_wakeup(buttonPin, 0);

  esp_sleep_enable_timer_wakeup(SLEEP_TIME_BETWEEN_FETCH);

}

void not_loop() {

  Serial.println("Going into light sleep mode");
  delay(500);

  esp_light_sleep_start();    // Enter light sleep

  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by BUTTON"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by TIMER"); break;
    default:                        Serial.printf("Wakeup cause spurious: %d\n", wakeup_reason); break;
  }


  
}
