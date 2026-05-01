//
// Gimi Piggy, Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_wifi.h"
#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_html_fetch.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_buttons.h"
#include "gimi_pb_qr204.h"

void setup() {

  Serial.begin(115200);

  Serial.println("GIMI PG STARTING");

//  piggy_html_fetch_setup();
  piggy_wifi_manager_setup();
  piggy_printer_setup();
  piggy_buttons_setup();
}

void loop() {
  piggy_wifi_manager_update();

  if (piggy_wifi_manager_is_connected() == true) {

//    piggy_html_fetch_update();
//    piggy_printer_update();

    if (piggy_button_0_debounced() == true) {

      gimi_pb_receipt_setup();
      
//      piggy_printer_update();
      

      
      size_t filesize = gimi_pb_receipt_get_filesize();
      uint32_t filesize_int = filesize;
      uint32_t fileheight_int = filesize_int / (PIXELS_PER_LINE / 8);

      // Check that a binary has been successfully downloaded,
      // and that the number of bytes is an exact multiple of PIXELS_PER_LINE.

      if(filesize_int > 0 && filesize_int % (PIXELS_PER_LINE / 8) == 0) {


        Serial.printf("Image dimensions size %d width %d height %d \n", filesize_int, PIXELS_PER_LINE, fileheight_int);
      
        printBitmap(gimi_pb_receipt_get_buffer(), PIXELS_PER_LINE, fileheight_int);
      }


    }
  }


}