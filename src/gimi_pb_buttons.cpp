//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_buttons.h"
#include "gimi_pb_pins.h"

const int buttonPin = GIMI_PB_GPIO_00; // BOOT Button pin. 
int buttonState = 0;

unsigned long sinceLastKey1Press = 0;
const unsigned long debounceDelay = 200;  // Debounce delay in milliseconds

void gimi_pb_buttons_setup() {
  pinMode(buttonPin, INPUT_PULLUP);
}

void gimi_pb_button_0() {

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
      Serial.println("BOOT BUTTON DOWN!!!");
  } else {
      Serial.println("BOOT BUTTON UP");
  }
}

bool gimi_pb_button_0_debounced() {

  if (digitalRead(buttonPin) == LOW) {

    if (millis() - sinceLastKey1Press > debounceDelay) {

      Serial.println("BOOT BUTTON DOWN!!!");
      sinceLastKey1Press = millis();

      return true;

    } else {
      return false;
    }

  } else  {
    return false;
  }
}
