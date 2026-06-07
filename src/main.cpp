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

#define WIFI_DELAY 64 // Milliseconds.

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
  delay(WIFI_DELAY);
  bool wifi_setup = gimi_pb_wifi_manager_setup();
  delay(WIFI_DELAY);
  gimi_pb_audio_setup();

  // Play the welcome LED and sound.
  if (wifi_setup == true)
    gimi_pb_state_machine_play_light_and_sound_sequence(DEFAULT_COLOUR_CODE, SOUND_BOOTUP);
  else
    gimi_pb_state_machine_play_light_and_sound_sequence(ERROR_COLOUR_CODE, SOUND_ERROR);

  // Light-sleep wake-up set-up.
  pinMode(GIMI_PB_GPIO_35, INPUT_PULLUP); // This is the main EARS button. Must use an external 10K pull-up resistor on ESP32-2432S028.
  esp_sleep_enable_ext0_wakeup((gpio_num_t)GIMI_PB_GPIO_35, 0); // Genereates EARS (EXT0) wake-up reason only if in light-sleep.
  esp_sleep_enable_timer_wakeup(SLEEP_TIME_BETWEEN_FETCH); // Genereates TIMER wake-up reason only if in light-sleep.

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

        attachInterrupt(GIMI_PB_GPIO_35, ear_button_ISR, FALLING); // Generates EARS button interrupt, but only during timer wake-up.
        Serial.println("Wakeup caused by TIMER");
        gimi_pb_state_machine_handle_timer();
        detachInterrupt(GIMI_PB_GPIO_35); // EARS interrupt disabled, to avoid clash with EXT0 wake-up.
        break;

      default:       

        Serial.printf("Wakeup cause spurious: %d\n", wakeup_reason);
        break;
    }
  }
}

