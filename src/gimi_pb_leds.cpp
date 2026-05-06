//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_leds.h"
#include "gimi_pb_pins.h"

// set pin numbers
const int redPin = GIMI_PB_GPIO_04;
const int greenPin = GIMI_PB_GPIO_16;
const int bluePin = GIMI_PB_GPIO_17;
const int backlightPin = GIMI_PB_GPIO_21;

void gimi_pb_leds_setup() {

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
//  pinMode(backlightPin, OUTPUT);

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
//  digitalWrite(backlightPin, HIGH);
}

void gimi_pb_leds_off() {

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void gimi_pb_leds_red() {

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void gimi_pb_leds_green() {

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void gimi_pb_leds_blue() {

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void gimi_pb_leds_yellow() {

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void gimi_pb_leds_magenta() {

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void gimi_pb_leds_cyan() {

  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void gimi_pb_leds_white() {

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

