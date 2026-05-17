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

#define RAMP_DELAY 4 // Milliseconds between each dimming step. E.g. 4 ms * 256 steps = ~1 second fade-in or fade-out.
#define HOLD_DELAY 1000 // Milliseconds delay to use as hold-time in place of playing a sound.

const uint32_t colour_table [] PROGMEM = { DEFAULT_COLOUR_VALUE, PAIRING_COLOUR_VALUE, CELEBRATION_EXCITED_COLOUR_VALUE, SUCCESS_COLOUR_VALUE, ERROR_COLOUR_VALUE, PAPER_OVER_COLOUR_VALUE };

void gimi_pb_leds_setup() {
  // set the LED as an output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
}

void gimi_pb_leds_ramp_up(uint32_t colour_code){

  uint32_t colour_table_value = colour_table[colour_code];

  uint8_t red = (uint8_t)((colour_table_value & 0x00ff0000) >> 16);
  uint8_t green = (uint8_t)((colour_table_value & 0x0000ff00) >> 8);
  uint8_t blue = (uint8_t)(colour_table_value & 0x000000ff);

  Serial.printf("Colour ramp-up %08x Red %02x Green %02x Blue %02x\n", colour_table_value, red, green, blue);

  uint8_t red_ramp = 0;
  uint8_t green_ramp = 0;
  uint8_t blue_ramp = 0;
  
  for(uint32_t i = 0; i < 256; i++){

    if (red_ramp < red)
      red_ramp++;
    if (green_ramp < green)
      green_ramp++;
    if (blue_ramp < blue)
      blue_ramp++;

    // Note that PWM values are inverted.
    analogWrite(RED_PIN, (uint8_t) ~red_ramp);
    analogWrite(GREEN_PIN, (uint8_t) ~green_ramp);
    analogWrite(BLUE_PIN, (uint8_t) ~blue_ramp);
    delay(RAMP_DELAY);
  }
}

void gimi_pb_leds_constant(uint32_t colour_code) {

  uint32_t colour_table_value = (colour_table[colour_code]);

  uint8_t red = (uint8_t)((colour_table_value & 0x00ff0000) >> 16);
  uint8_t green = (uint8_t)((colour_table_value & 0x0000ff00) >> 8);
  uint8_t blue = (uint8_t)(colour_table_value & 0x000000ff);

  Serial.printf("Colour constant %08x Red %02x Green %02x Blue %02x\n", colour_table_value, red, green, blue);

  // Note that PWM values are inverted.
  analogWrite(RED_PIN, (uint8_t) ~red);
  analogWrite(GREEN_PIN, (uint8_t) ~green);
  analogWrite(BLUE_PIN, (uint8_t) ~blue);
}

void gimi_pb_leds_hold(void) {

  Serial.printf("Colour hold time %d milliseconds\n", HOLD_DELAY);
  delay(HOLD_DELAY);
}

void gimi_pb_leds_ramp_down(uint32_t colour_code) {


  uint32_t colour_table_value = colour_table[colour_code];

  uint8_t red = (uint8_t)((colour_table_value & 0x00ff0000) >> 16);
  uint8_t green = (uint8_t)((colour_table_value & 0x0000ff00) >> 8);
  uint8_t blue = (uint8_t)(colour_table_value & 0x000000ff);

  Serial.printf("Colour ramp-down %08x Red %02x Green %02x Blue %02x\n", colour_table_value, red, green, blue);

  uint8_t red_ramp = red;
  uint8_t green_ramp = green;
  uint8_t blue_ramp = blue;
    
  for(uint32_t i = 0; i < 256; i++){

    if (red_ramp > 0)
      red_ramp--;
    if (green_ramp > 0)
      green_ramp--;
    if (blue_ramp > 0)
      blue_ramp--;

    // Note that PWM values are inverted.
    analogWrite(RED_PIN, (uint8_t) ~red_ramp);
    analogWrite(GREEN_PIN, (uint8_t) ~green_ramp);
    analogWrite(BLUE_PIN, (uint8_t) ~blue_ramp);
    delay(RAMP_DELAY);
  }
}

void gimi_pb_leds_off() {

  // Reset pins to OFF state.
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, HIGH);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, HIGH);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(BLUE_PIN, HIGH);
}
