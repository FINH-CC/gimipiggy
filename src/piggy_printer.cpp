//
// Gimi Piggy, Copyright 2026.
//

#include <HardwareSerial.h>

#include "piggy_printer.h"
#include "piggy_experimental_printer.h"
#include "piggy_gimi_logo.h"

uint32_t printer_repeat_counter;
#define PRINTER_REPEAT_COUNT 20000

void piggy_printer_setup() {

  piggy_experimental_printer_setup();

  printer_repeat_counter = PRINTER_REPEAT_COUNT; // Ensure print happens on first pass.
}

void piggy_printer_update() {

  if(printer_repeat_counter == PRINTER_REPEAT_COUNT) {

//	setDarknessAndDelay(100, 1000);
//	setDarknessAndDelay(50, 1750);

    printBitmapGS_Method(epd_bitmap_gimi_small_logo, 77, 32);
    printBitmapGS_Method(epd_bitmap_gimi_logo, 308, 128);
    //printBitmapGS_Method(epd_bitmap_Celebration_Receipt, 384, 1175);
    printBitmapGS_Method(epd_bitmap_W384_Type_Test, 384, 1047);

//	demoPrintAllFormats();

    printer_repeat_counter = 0;
  }

  printer_repeat_counter++;
}
