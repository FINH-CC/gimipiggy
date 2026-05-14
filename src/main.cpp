//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_audio.h"
#include "gimi_pb_buttons.h"
#include "gimi_pb_leds.h"
#include "gimi_pb_pins.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_server.h"
#include "gimi_pb_state_machine.h"
#include "gimi_pb_wifi.h"

void ARDUINO_ISR_ATTR ear_button_ISR() {

  gimi_pb_state_machine_set_button_pressed();
}

void setup() {

  Serial.begin(115200);

  Serial.println("GIMI PG STARTING");

  // General set-up.
  gimi_pb_leds_setup();
  gimi_pb_buttons_setup();
  gimi_pb_state_machine_setup();
  gimi_pb_bin_file_setup();
  gimi_pb_wifi_manager_setup();
  gimi_pb_audio_setup();

  // Timer wake-up set-up.
  esp_sleep_enable_timer_wakeup(SLEEP_TIME_BETWEEN_FETCH); // Genereates TIMER wake-up reason only if in light-sleep.

  // EARS button wake-up and interrupt (both needed, in case button pressed during binary download, LED effects or Audio effects).

  pinMode(GIMI_PB_GPIO_35, INPUT_PULLUP); // This is the main EARS button. Must use an external 10K pull-up resistor on ESP32-2432S028.
  delay(1000); // There may be some rise-time, so insert a delay before attaching the interrupt.

  esp_sleep_enable_ext0_wakeup((gpio_num_t)GIMI_PB_GPIO_35, 0); // Genereates EARS (EXT0) wake-up reason only if in light-sleep.
  attachInterrupt(GIMI_PB_GPIO_35, ear_button_ISR, FALLING); // Generates EARS button interrupt, if device is running, AND in addition to EXT0 wake-up.

  // Printed after a reset or power-on.
  // Prints the welcome and set-up instructions. These are stored on the device in flash, as wifi may not yet have been set up.
  gimi_pb_printer_begin();
  gimi_pb_printer_print_base64();
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
        gimi_pb_state_machine_handle_button();
        break;

      case ESP_SLEEP_WAKEUP_TIMER:  

        Serial.println("Wakeup caused by TIMER");
        gimi_pb_state_machine_handle_timer();
        break;

      default:       

        Serial.printf("Wakeup cause spurious: %d\n", wakeup_reason);
        break;
    }
  }
}

