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
#include "gimi_pb_pins.h"

#define gimi_pg_main_button  GIMI_PB_GPIO_35 // This is the main EARS button.

void setup() {

  Serial.begin(115200);

  Serial.println("GIMI PG STARTING");

  gimi_pb_wifi_manager_setup();
  gimi_pb_bin_file_setup();
  gimi_pb_buttons_setup();
  gimi_pb_leds_setup();

  pinMode(gimi_pg_main_button, INPUT_PULLUP); // Must use an external 10K pull-up resistor on ESP32-2432S028.
  esp_sleep_enable_ext0_wakeup((gpio_num_t)gimi_pg_main_button, 0);
  esp_sleep_enable_timer_wakeup(SLEEP_TIME_BETWEEN_FETCH);

  // Print the welcome and set-up instructions. These are stored on the device in flash, as wifi may not yet have been set up.
  gimi_pb_printer_begin();
//  gimi_pb_printer_print_base64();
  gimi_pb_printer_end();
}

void loop() {

  if (gimi_pb_wifi_manager_is_config_mode() == true) {

    // Config mode, so just stay awake and connected.
    gimi_pb_wifi_manager_reconnect();

  } else {

    Serial.println("Going into light sleep mode");
    delay(500);
    esp_light_sleep_start();

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason) {

      case ESP_SLEEP_WAKEUP_EXT0:   

        Serial.println("Wakeup caused by BUTTON"); 

        // If the BOOT button is being held down when the EARS button is pressed, then clear the WiFi settings and reboot.
        if (gimi_pb_button_0_debounced() == true)
          gimi_pb_wifi_manager_restart_wifi_setup();

        // Otherwise print the recipt, if one is available.
        gimi_pb_printer_begin();
        gimi_pb_printer_print_binary();
        gimi_pb_printer_end();
        gimi_pb_leds_off();

        break;

      case ESP_SLEEP_WAKEUP_TIMER:  

        Serial.println("Wakeup caused by TIMER");

        // If there's already a receipt ready to print, then don't bother to search for more until the current receipt has been printed.
        if (gimi_pb_get_bin_file_available() == false) {

          gimi_pb_wifi_manager_reconnect();

          if (gimi_pb_wifi_manager_is_connected() == true)
            gimi_pb_bin_file_update();

          gimi_pb_wifi_manager_disconnect();

          // If there is a receipt ready print, after the update, then turn on the indicator.
          if (gimi_pb_get_bin_file_available() == true)
            gimi_pb_leds_green();

        }

        break;

      default:       

        Serial.printf("Wakeup cause spurious: %d\n", wakeup_reason);

        break;
    }
  }
}

