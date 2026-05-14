//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_state_machine.h"
#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_buttons.h"
#include "gimi_pb_leds.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_wifi.h"

volatile bool ear_button_pressed = false;

// The ESP32 is either running, or in light-sleep.
//
// There are 2 event sources, the timer, or the 'ears' button (actually 2 buttons, wired in parallel).
//
// Timer events only occur during light-sleep.
// On wake-up, a check is made whether anything is already in the print-buffer and ready to print.
// If yes, then the appropriate LED's and sound sequence is played (depnding on the receipt type).
// If nothing is ready to print (i.e. whatever is in the buffer has already been printed),
// then a check is made on the server.
// If nothing new is found on the server, then this device returns silently to sleep.
// If a new item has been downloaded to the print buffer, then the appropriate LED's and sound sequence is played.
//
// Button events may occur either when the device is in light-sleep, or whilst running.
// In either case, a check should be made if there is anything not-yet-printed in the print buffer,
// the item printed, the print-buffer item marked as already-printed, and the device return to light-sleep.

void gimi_pb_state_machine_setup(void) {

  ear_button_pressed = false;
}

void gimi_pb_state_machine_handle_timer(void) {

  // Clear any pending button presses. It should not be possible to 'anticipate' a printable receipt.
  gimi_pb_state_machine_clear_button_pressed();

  // If there's already a receipt ready to print, then don't bother to search for more until the current receipt has been printed.
  if (gimi_pb_get_bin_file_available() == false) {

    gimi_pb_wifi_manager_reconnect();

    if (gimi_pb_wifi_manager_is_connected() == true)
      gimi_pb_bin_file_update();

    gimi_pb_wifi_manager_disconnect();

    // If there is a receipt ready print, after the update, then turn on the indicator.
    if (gimi_pb_get_bin_file_available() == true) {

//            gimi_pb_leds_green();
      gimi_pb_leds_colour_pulse(CELEBRATION_EXCITED_COLOUR);

    }

  } else {
    gimi_pb_leds_colour_pulse(CELEBRATION_EXCITED_COLOUR);
  }

  // Finally check whether the EARS button has been pressed during this handler.
  if (gimi_pb_state_machine_get_button_pressed() == true)
    gimi_pb_state_machine_handle_button();
}

void gimi_pb_state_machine_handle_button(void) {

  // May arrive at this point through a button push during light-sleep (EXT0), or a button press whilst running, and handling a timer event.
  // Handle it the same way, then clear the EARS button pressed flag. 

  // If the BOOT button is being held down when the EARS button is pressed, then clear the WiFi settings and reboot.
  if (gimi_pb_button_0_debounced() == true)
    gimi_pb_wifi_manager_restart_wifi_setup();

  // Otherwise print the recipt, if one is available.
  if (gimi_pb_get_bin_file_available()) {

    gimi_pb_printer_begin();
    gimi_pb_printer_print_binary();
    gimi_pb_printer_end();
    gimi_pb_leds_off();

  } else {

    // If no new receipt is available, then fetch and print the Default receipt.
    gimi_pb_wifi_manager_reconnect();

    if (gimi_pb_wifi_manager_is_connected() == true) {

      if (gimi_pb_bin_file_default_get() == true) {

        gimi_pb_printer_begin();
        gimi_pb_printer_print_binary();
        gimi_pb_printer_end();
        gimi_pb_leds_off();
      }
    }
  
    gimi_pb_wifi_manager_disconnect();
}

  gimi_pb_state_machine_clear_button_pressed();
}

void gimi_pb_state_machine_set_button_pressed(void) { 

  ear_button_pressed = true;
  Serial.println("EAR BUTTON INTERRUPT - button press SET");
}

void gimi_pb_state_machine_clear_button_pressed(void) {

  ear_button_pressed = false;
  Serial.println("Button press CLEARED");
}

bool gimi_pb_state_machine_get_button_pressed(void) {
    
  return ear_button_pressed;
}

