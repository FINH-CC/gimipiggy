//
// Copyright 2026.
//

#include <Arduino.h>
#include <float.h>
#include <driver/ledc.h>

#include "gimi_pb_leds.h"
#include "gimi_pb_pins.h"

#define PWM_CHANNEL_RED   LEDC_CHANNEL_0
#define PWM_CHANNEL_GREEN LEDC_CHANNEL_1
#define PWM_CHANNEL_BLUE  LEDC_CHANNEL_2

#define PWM_FREQ    5000             // Hz
#define PWM_RES     LEDC_TIMER_8_BIT // 8-bit = 0–255

// set pin numbers
#define RED_PIN     GIMI_PB_GPIO_19
#define GREEN_PIN   GIMI_PB_GPIO_18
#define BLUE_PIN    GIMI_PB_GPIO_05

#define PWM_MAX     255

#define RED_MASK    0x00ff0000
#define GREEN_MASK  0x0000ff00
#define BLUE_MASK   0x000000ff

#define BIT_MASK_8  0x000000ff

#define RED_SHIFT   16
#define GREEN_SHIFT 8
#define BLUE_SHIFT  0

#define RAMP_DELAY 64 // Milliseconds between each dimming step.
#define RAMP_STEPS 16
#define HOLD_DELAY 1000 // Milliseconds delay to use as hold-time in place of playing a sound.

const uint32_t colour_table [] PROGMEM = { DEFAULT_COLOUR_VALUE, PAIRING_COLOUR_VALUE, CELEBRATION_EXCITED_COLOUR_VALUE, SUCCESS_COLOUR_VALUE, ERROR_COLOUR_VALUE, PAPER_OVER_COLOUR_VALUE };

void gimi_pb_leds_setup() {

  gpio_set_direction((gpio_num_t)RED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)RED_PIN, 1);

  gpio_set_direction((gpio_num_t)GREEN_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)GREEN_PIN, 1);

  gpio_set_direction((gpio_num_t)BLUE_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)BLUE_PIN, 1);

  ledc_timer_config_t timer_red = {
    .speed_mode       = LEDC_LOW_SPEED_MODE,
    .duty_resolution  = PWM_RES,
    .timer_num        = LEDC_TIMER_0,
    .freq_hz          = PWM_FREQ,
    .clk_cfg          = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_red);

  ledc_timer_config_t timer_green = {
    .speed_mode       = LEDC_LOW_SPEED_MODE,
    .duty_resolution  = PWM_RES,
    .timer_num        = LEDC_TIMER_1,
    .freq_hz          = PWM_FREQ,
    .clk_cfg          = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_green);

  ledc_timer_config_t timer_blue = {
    .speed_mode       = LEDC_LOW_SPEED_MODE,
    .duty_resolution  = PWM_RES,
    .timer_num        = LEDC_TIMER_2,
    .freq_hz          = PWM_FREQ,
    .clk_cfg          = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_blue);

}

void gimi_pb_leds_on() {

  ledc_channel_config_t channel_red = {
    .gpio_num   = RED_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = PWM_CHANNEL_RED,
    .timer_sel  = LEDC_TIMER_0,
    .duty       = 0,
    .hpoint     = 0
  };

  ledc_channel_config(&channel_red);

  ledc_channel_config_t channel_green = {
    .gpio_num   = GREEN_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = PWM_CHANNEL_GREEN,
    .timer_sel  = LEDC_TIMER_1,
    .duty       = 0,
    .hpoint     = 0
  };

  ledc_channel_config(&channel_green);

  ledc_channel_config_t channel_blue = {
    .gpio_num   = BLUE_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel    = PWM_CHANNEL_BLUE,
    .timer_sel  = LEDC_TIMER_2,
    .duty       = 0,
    .hpoint     = 0
  };

  ledc_channel_config(&channel_blue);

}

void gimi_pb_leds_off() {

  // Detach pin from LEDC and drive it high (active-low = LED off)

  ledc_stop(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_RED, 1); // idle level = 1
  gpio_set_direction((gpio_num_t)RED_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)RED_PIN, 1);

  ledc_stop(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_GREEN, 1); // idle level = 1
  gpio_set_direction((gpio_num_t)GREEN_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)GREEN_PIN, 1);

  ledc_stop(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_BLUE, 1); // idle level = 1
  gpio_set_direction((gpio_num_t)BLUE_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)BLUE_PIN, 1);
}

void gimi_pb_leds_ramp_up(uint32_t colour_code){

  uint32_t colour_table_value = colour_table[colour_code];

  uint32_t red =   (colour_table_value & RED_MASK)   >> RED_SHIFT;
  uint32_t green = (colour_table_value & GREEN_MASK) >> GREEN_SHIFT;
  uint32_t blue =  (colour_table_value & BLUE_MASK)  >> BLUE_SHIFT;

  Serial.printf("Colour ramp-up %08x Red %08x Green %08x Blue %08x\n", colour_table_value, red, green, blue);

  uint32_t red_ramp;
  uint32_t green_ramp;
  uint32_t blue_ramp;
  
  for (uint32_t i = 0; i < RAMP_STEPS; i++) {

    red_ramp   = (uint32_t)(((float)red *   (float)i) / (float)RAMP_STEPS);
    green_ramp = (uint32_t)(((float)blue *  (float)i) / (float)RAMP_STEPS);
    blue_ramp  = (uint32_t)(((float)green * (float)i) / (float)RAMP_STEPS);

    // Note that PWM values are inverted.

    set_red_duty((~red_ramp) & BIT_MASK_8);
    set_green_duty((~green_ramp) & BIT_MASK_8);
    set_blue_duty((~blue_ramp) & BIT_MASK_8);

    delay(RAMP_DELAY);
  }

  set_red_duty(red);
  set_green_duty(green);
  set_blue_duty(blue);

}

void gimi_pb_leds_constant(uint32_t colour_code) {

  uint32_t colour_table_value = colour_table[colour_code];

  uint32_t red =   (colour_table_value & RED_MASK)   >> RED_SHIFT;
  uint32_t green = (colour_table_value & GREEN_MASK) >> GREEN_SHIFT;
  uint32_t blue =  (colour_table_value & BLUE_MASK)  >> BLUE_SHIFT;

  Serial.printf("Colour constant %08x Red %02x Green %02x Blue %02x\n", colour_table_value, red, green, blue);

  // Note that PWM values are inverted.

  set_red_duty((~red) & BIT_MASK_8);
  set_green_duty((~green) & BIT_MASK_8);
  set_blue_duty((~blue) & BIT_MASK_8);
}

void gimi_pb_leds_hold(void) {

  Serial.printf("Colour hold time %d milliseconds\n", HOLD_DELAY);
  delay(HOLD_DELAY);
}

void gimi_pb_leds_ramp_down(uint32_t colour_code) {


  uint32_t colour_table_value = colour_table[colour_code];

  uint8_t red = (uint8_t)((colour_table_value & RED_MASK) >> RED_SHIFT);
  uint8_t green = (uint8_t)((colour_table_value & GREEN_MASK) >> GREEN_SHIFT);
  uint8_t blue = (uint8_t)(colour_table_value & BLUE_MASK);

  Serial.printf("Colour ramp-down %08x Red %02x Green %02x Blue %02x\n", colour_table_value, red, green, blue);

  uint32_t red_ramp;
  uint32_t green_ramp;
  uint32_t blue_ramp;
  
  for (uint32_t i = RAMP_STEPS; i > 0; i--) {

    red_ramp   = (uint32_t)(((float)red *   (float)i) / (float)RAMP_STEPS);
    green_ramp = (uint32_t)(((float)blue *  (float)i) / (float)RAMP_STEPS);
    blue_ramp  = (uint32_t)(((float)green * (float)i) / (float)RAMP_STEPS);

    // Note that PWM values are inverted.

    set_red_duty((~red_ramp) & BIT_MASK_8);
    set_green_duty((~green_ramp) & BIT_MASK_8);
    set_blue_duty((~blue_ramp) & BIT_MASK_8);

    delay(RAMP_DELAY);
  }

  set_red_duty(PWM_MAX);
  set_green_duty(PWM_MAX);
  set_blue_duty(PWM_MAX);

}

void set_red_duty(uint32_t duty) {
  ledc_set_duty(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_RED, duty);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_RED);
}

void set_green_duty(uint32_t duty) {
  ledc_set_duty(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_GREEN, duty);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_GREEN);
}

void set_blue_duty(uint32_t duty) {
  ledc_set_duty(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_BLUE, duty);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_BLUE);
}
