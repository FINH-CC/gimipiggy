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

void gimi_pb_leds_colour_pulse(uint32_t colour_code){

  uint32_t colour_table_value = colour_table[colour_code];

  uint8_t red = (uint8_t)((colour_table_value & 0x00ff0000) >> 16);
  uint8_t green = (uint8_t)((colour_table_value & 0x0000ff00) >> 8);
  uint8_t blue = (uint8_t)(colour_table_value & 0x000000ff);

  Serial.printf("Colour %08x Red %02x Green %02x Blue %02x\n", colour_table_value, red, green, blue);


  for (uint32_t flashes = 0; flashes < 8; flashes++) {
    for(int dutyCycle = 224; dutyCycle >= 0; dutyCycle--){

      uint32_t red_scaled = ((red * dutyCycle) >> 8);
      uint32_t green_scaled = ((green * dutyCycle) >> 8);
      uint32_t blue_scaled = ((blue * dutyCycle) >> 8);
      
      analogWrite(RED_PIN, (uint8_t) red_scaled);
      analogWrite(GREEN_PIN, (uint8_t) green_scaled);
      analogWrite(BLUE_PIN, (uint8_t) blue_scaled);
      delay(5);
    }

    for(int dutyCycle = 0; dutyCycle <= 224; dutyCycle++){   
      
      uint32_t red_scaled = ((red * dutyCycle) >> 8);
      uint32_t green_scaled = ((green * dutyCycle) >> 8);
      uint32_t blue_scaled = ((blue * dutyCycle) >> 8);
      
      analogWrite(RED_PIN, (uint8_t) red_scaled);
      analogWrite(GREEN_PIN, (uint8_t) green_scaled);
      analogWrite(BLUE_PIN, (uint8_t) blue_scaled);
      delay(5);
    }
  }

  // Reset pins to OFF state.
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, HIGH);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, HIGH);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(BLUE_PIN, HIGH);
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

