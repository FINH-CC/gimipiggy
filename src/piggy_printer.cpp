//
// Gimi Piggy, Copyright 2026.
//

#include <HardwareSerial.h>
#include "piggy_printer.h"

uint32_t printer_repeat_counter;
#define PRINTER_REPEAT_COUNT 20000

HardwareSerial mySerial(2);  // use UART2 (GPIO22 TX, GPIO27 RX)

void piggy_printer_setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 22, 27); // RX, TX

  delay(2000);
  
  printer_repeat_counter = PRINTER_REPEAT_COUNT; // Ensure print happens on first pass.
}

void piggy_printer_update() {

  if(printer_repeat_counter == PRINTER_REPEAT_COUNT) {


    // Print a test message
    mySerial.println("=== Thermal Printer Test ===");
    mySerial.println("Hello from the ESP32!");
    mySerial.println("This is a thermal printer demo.");

      printer_repeat_counter = 0;
  }

  printer_repeat_counter++;
}