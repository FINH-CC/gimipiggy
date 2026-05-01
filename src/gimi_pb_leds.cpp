//
// Gimi Piggy, Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_leds.h"

// set pin numbers
const int redPin = 4;
const int greenPin = 16;
const int bluePin = 17;
const int backlightPin = 21;

void piggy_leds_setup() {

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
//  pinMode(backlightPin, OUTPUT);

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
//  digitalWrite(backlightPin, HIGH);
}

void piggy_leds_update() {
}

