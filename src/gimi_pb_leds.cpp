//
// Copyright 2026.
//

#include <Arduino.h>

#include "gimi_pb_leds.h"
#include "gimi_pb_pins.h"

// set pin numbers
#define RED_PIN GIMI_PB_GPIO_04
#define GREEN_PIN GIMI_PB_GPIO_16
#define BLUE_PIN GIMI_PB_GPIO_17
#define PRINTER_POWER_CONTROL GIMI_PB_GPIO_21 // Note that this is also the backlight pin.

void gimi_pb_leds_setup() {

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(PRINTER_POWER_CONTROL, OUTPUT);

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
  digitalWrite(PRINTER_POWER_CONTROL, LOW);
}

void gimi_pb_leds_off() {

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void gimi_pb_leds_red() {

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void gimi_pb_leds_green() {

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}

void gimi_pb_leds_blue() {

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
}

void gimi_pb_leds_yellow() {

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}

void gimi_pb_leds_magenta() {

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
}

void gimi_pb_leds_cyan() {

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void gimi_pb_leds_white() {

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

