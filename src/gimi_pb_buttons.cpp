//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_buttons.h"
#include "gimi_pb_pins.h"

// set pin numbers
const int buttonPin = GPIO_35;  // Pushbutton pin.

// variable for storing the pushbutton status 
int buttonState = 0;

unsigned long sinceLastKey1Press = 0;      // Last time Key1 was pressed (for debouncing)
const unsigned long debounceDelay = 200;  // Button debounce delay in milliseconds

void gimi_pb_buttons_setup() {
  pinMode(buttonPin, INPUT_PULLUP);
}

void gimi_pb_button_0() {
  // read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);
//  Serial.println(buttonState);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH
  if (buttonState == LOW) {
      Serial.println("BUTTON DOWN!!!");
  } else {
      Serial.println("BUTTON UP");
  }
}

bool gimi_pb_button_0_debounced() {

 // Button handling with debouncing
  if (digitalRead(buttonPin) == LOW) {

    if (millis() - sinceLastKey1Press > debounceDelay) {

      Serial.println("BUTTON DOWN!!!");
      sinceLastKey1Press = millis();

      return true;

    } else {
      return false;
    }

  } else  {
    return false;
  }
}
