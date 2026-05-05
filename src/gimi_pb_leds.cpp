//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_leds.h"
#include "gimi_pb_pins.h"

// set pin numbers
const int redPin = GPIO_04;
const int greenPin = GPIO_16;
const int bluePin = GPIO_17;
const int backlightPin = GPIO_21;

void gimi_pb_leds_setup() {

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
//  pinMode(backlightPin, OUTPUT);

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
//  digitalWrite(backlightPin, HIGH);
}

void gimi_pb_leds_update() {
}

