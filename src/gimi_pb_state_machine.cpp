//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_state_machine.h"
#include "gimi_pb_audio.h"
#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_buttons.h"
#include "gimi_pb_leds.h"
#include "gimi_pb_pins.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_server.h"
#include "gimi_pb_wifi.h"

#define FAST_CLICK_COUNT_THRESHOLD 5 // Number of fast clicks on the ear-buttons required to reset WiFi.
#define FAST_CLICK_TIMEOUT 2000 // The time interval, in ms, in which the fast clicks must occur.
#define FAST_CLICK_DEBOUNCE_DELAY 50 // Interval between re-checking for new clicks.

#define HOLD_CLICK_TIME_THRESHOLD 2000 // The time interval, in ms, for which the last click must be sustained.
#define HOLD_CLICK_TEST_DURATION 5000 // The maximum time interval, in ms, for which for which to check for a long-click.

volatile bool ear_button_pressed = false;

uint32_t fast_click_count;
uint32_t fast_click_start_time;
bool fast_click_threshold_met;

uint32_t hold_click_test_start_time;
uint32_t hold_click_start_time;

void gimi_pb_state_machine_setup(void) {

  ear_button_pressed = false;
}

void gimi_pb_state_machine_handle_timer(void) {

  Serial.printf("State Machine handling timer event.\n");

  // Clear any pending button presses. It should not be possible to 'anticipate' a printable receipt.
  gimi_pb_state_machine_clear_button_pressed();

  // If there's already a receipt ready to print, then don't bother to search for more until the current receipt has been printed.
  if (gimi_pb_get_bin_new_file_available() == false || gimi_pb_get_bin_file_downloaded() == false ) {

    Serial.printf("State Machine - timer - about to look for new file on server.\n");

    gimi_pb_wifi_manager_reconnect();

    if (gimi_pb_wifi_manager_is_connected() == true) {

      Serial.printf("State Machine - timer - WiFi reconnect successful, and about to update receipts.\n");
      gimi_pb_bin_file_timer_initiated_update();
    }

    gimi_pb_wifi_manager_disconnect();

    // If there is a new receipt ready to print, after the update, announce it.
    if (gimi_pb_get_bin_new_file_available() == true && gimi_pb_get_bin_file_downloaded() == true) {

      Serial.printf("State Machine - WiFi disconnected, and new receipt type available.\n");
      gimi_pb_state_machine_play_sequence_by_receipt_type(gimi_pb_get_file_type(), true);
    }

  } else {

    Serial.printf("State Machine - exisiting unprinted receipt available\n");
    // If there is already an existing receipt ready print, anounce it again, but without sound.
    gimi_pb_state_machine_play_sequence_by_receipt_type(gimi_pb_get_file_type(), false);
  }

  // Finally check whether the EARS button has been pressed during this handler.
  if (gimi_pb_state_machine_get_button_pressed() == true) {

    Serial.printf("State Machine - button press during timer handler.\n");
    gimi_pb_state_machine_handle_button();
  }
}

void gimi_pb_state_machine_handle_button(void) {

  // May arrive at this point through a button push during light-sleep (EXT0), or a button press whilst running, and handling a timer event.
  // Handle it the same way, then clear the EARS button pressed flag. 

  // If the BOOT button is being held down when the EARS button is pressed, then clear the WiFi settings and reboot.

  Serial.printf("State Machine - handling button event.\n");

  if (gimi_pb_button_0_debounced() == true) {

    Serial.printf("State Machine - BOOT button press. About to reset WiFi.\n");
    gimi_pb_wifi_manager_restart_wifi_setup();
  }

  fast_click_threshold_met = false;
  fast_click_count = 0;
  fast_click_start_time = millis();
  gimi_pb_state_machine_clear_button_pressed();

  while (millis() < (fast_click_start_time + FAST_CLICK_TIMEOUT)) {

    if (gimi_pb_state_machine_get_button_pressed() == true) {

      gimi_pb_state_machine_clear_button_pressed();
      fast_click_count++;
      Serial.printf("FAST CLICK COUNT %d\n", fast_click_count);
    
    }

    if (fast_click_count >= FAST_CLICK_COUNT_THRESHOLD) {
      Serial.printf("FAST CLICK THRESHOLD MET\n");
      fast_click_threshold_met = true;
      break; // Move on immediately to detection of the long-click.
    }

    delay(FAST_CLICK_DEBOUNCE_DELAY);
  }

  if (fast_click_threshold_met == true) {

    Serial.printf("WAITING FOR LONG CLICK\n");
    hold_click_test_start_time = millis();

    while (millis() < (hold_click_test_start_time + HOLD_CLICK_TEST_DURATION)) {

      hold_click_start_time = millis();

      while(digitalRead(GIMI_PB_GPIO_35) == LOW) {
        if ((millis() - hold_click_start_time)  > HOLD_CLICK_TIME_THRESHOLD) {          
          Serial.printf("LONG PRESS DURATION THRESHOLD MET. About to reset WiFi.\n");
          gimi_pb_wifi_manager_restart_wifi_setup(); // Reset WiFi.
        }
      }

    }

  }

  // Otherwise print the receipt.
  
  bool print_successful = false;

  print_successful = gimi_pb_bin_print_downloaded_receipt();

  if (print_successful == true) {

    Serial.printf("State Machine - button event - printing from existing download completed.\n");
    gimi_pb_state_machine_play_sound_only_sequence(SOUND_SUCCESS);

  } else {

    Serial.printf("State Machine - button event - printing failed.\n");
    gimi_pb_state_machine_play_sound_only_sequence(SOUND_ERROR);
  }

  gimi_pb_state_machine_clear_button_pressed();
}

void gimi_pb_state_machine_play_sequence_by_receipt_type(uint32_t receipt_type, bool with_sound) {

  uint32_t light_colour;
  uint32_t sound_clip;

  if (receipt_type == GIMI_PB_RECEIPT_TYPE_WELCOME) {

    light_colour = DEFAULT_COLOUR_CODE;
    sound_clip = SOUND_CELEBRATION;

  } else if (receipt_type == GIMI_PB_RECEIPT_TYPE_DEFAULT) {

    light_colour = DEFAULT_COLOUR_CODE;
    sound_clip = SOUND_COIN_CLINK;

  } else if (receipt_type == GIMI_PB_RECEIPT_TYPE_SAVINGS) {

    light_colour = CELEBRATION_EXCITED_COLOUR_CODE;
    sound_clip = SOUND_EXCITED_OINK;

  } else if (receipt_type == GIMI_PB_RECEIPT_TYPE_CELEBRATION) {

    light_colour = CELEBRATION_EXCITED_COLOUR_CODE;
    sound_clip = SOUND_MONEY_ADDED;

  } else if (receipt_type == GIMI_PB_RECEIPT_TYPE_REMINDER) {

    light_colour = CELEBRATION_EXCITED_COLOUR_CODE;
    sound_clip = SOUND_NOTIFICATION_OINK;

  } else if (receipt_type == GIMI_PB_RECEIPT_TYPE_NOTIFICATION) {

    light_colour = CELEBRATION_EXCITED_COLOUR_CODE;
    sound_clip = SOUND_SUCCESS;

  } else {
    // Unknown receipt type!
    light_colour = DEFAULT_COLOUR_CODE;
    sound_clip = SOUND_ERROR;
  }

  if (with_sound == true) {
    gimi_pb_state_machine_play_light_and_sound_sequence(light_colour, sound_clip);
  } else {
    gimi_pb_state_machine_play_light_only_sequence(light_colour);
  }

}

void gimi_pb_state_machine_play_light_and_sound_sequence(uint32_t light, uint32_t sound) {

  gimi_pb_leds_on();
  gimi_pb_leds_ramp_up(light);
  gimi_pb_leds_constant(light);
  gimi_pb_audio_play(sound);
  gimi_pb_leds_ramp_down(light);
  gimi_pb_leds_off();
}

void gimi_pb_state_machine_play_light_only_sequence(uint32_t light) {

  gimi_pb_leds_on();
  gimi_pb_leds_ramp_up(light);
  gimi_pb_leds_constant(light);
  gimi_pb_leds_hold();
  gimi_pb_leds_ramp_down(light);
  gimi_pb_leds_off();
}

void gimi_pb_state_machine_play_sound_only_sequence(uint32_t sound) {

  gimi_pb_audio_play(sound);
}

void gimi_pb_state_machine_printer_pre_sequence(uint32_t light) {

  gimi_pb_leds_on();
  gimi_pb_leds_ramp_up(light);
  gimi_pb_leds_constant(light);
}

void gimi_pb_state_machine_printer_post_sequence(uint32_t light, uint32_t sound) {

  gimi_pb_audio_play(sound);
  gimi_pb_leds_ramp_down(light);
  gimi_pb_leds_off();
}

void gimi_pb_state_machine_set_button_pressed(void) { 

  ear_button_pressed = true;
}

void gimi_pb_state_machine_clear_button_pressed(void) {

  ear_button_pressed = false;
  Serial.println("Button press CLEARED");
}

bool gimi_pb_state_machine_get_button_pressed(void) {
    
  return ear_button_pressed;
}

