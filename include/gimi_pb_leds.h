//
// Copyright 2026.
//

#ifndef __GIMI_PB_LEDS_H
#define __GIMI_PB_LEDS_H

enum Color { DEFAULT_COLOUR = 0, PAIRING_COLOUR, CELEBRATION_EXCITED_COLOUR, SUCCESS_COLOUR, ERROR_COLOUR, PAPER_OVER_COLOUR};

#define DEFAULT_COLOUR_VALUE             0x00FFFFFF
#define PAIRING_COLOUR_VALUE             0x000DEBEB
#define CELEBRATION_EXCITED_COLOUR_VALUE 0x00FFDBF4
#define SUCCESS_COLOUR_VALUE             0x0000FF00 //0x005CD242
#define ERROR_COLOUR_VALUE               0x00E34B4B
#define PAPER_OVER_COLOUR_VALUE          0x00FF9E42

// LED events are split into 3 parts, ramp-up, constant colour, ramp-down.
// This is designed to be used in conjunction with sounds, as ramp-up and ramp-down do not immediately return, but constant colout does.
//
// Recommended sequence:
//
// Call ramp-up.
// Set constant colour (returns immediately).
// Play sound.
// Call ramp-down.

void gimi_pb_leds_setup(void);

void gimi_pb_leds_off(void);

void gimi_pb_leds_colour_pulse(uint32_t colour_code);

void gimi_pb_leds_red(void);
void gimi_pb_leds_green(void);
void gimi_pb_leds_blue(void);
void gimi_pb_leds_yellow(void);
void gimi_pb_leds_magenta(void);
void gimi_pb_leds_cyan(void);
void gimi_pb_leds_white(void);

#endif // __GIMI_PB_LEDS_H
