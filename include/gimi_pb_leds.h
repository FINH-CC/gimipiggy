//
// Copyright 2026.
//

#ifndef __GIMI_PB_LEDS_H
#define __GIMI_PB_LEDS_H

enum gimi_pb_colour {
    DEFAULT_COLOUR_CODE = 0,
    PAIRING_COLOUR_CODE,
    CELEBRATION_EXCITED_COLOUR_CODE,
    SUCCESS_COLOUR_CODE,
    ERROR_COLOUR_CODE,
    PAPER_OVER_COLOUR_CODE
};

// The ESP32-2432S028 has very low drive on Green, so use adapted values.

/*#define DEFAULT_COLOUR_VALUE             0x001FFF3F // White.
#define PAIRING_COLOUR_VALUE             0x0000FF3F // Light blue.
#define CELEBRATION_EXCITED_COLOUR_VALUE 0x003FFF7F // Pink.
#define SUCCESS_COLOUR_VALUE             0x0000FF00 // Green.
#define ERROR_COLOUR_VALUE               0x00FF0000 // Red.
#define PAPER_OVER_COLOUR_VALUE          0x007FFF00 // Amber.*/

// Standard colour values from Figma.

#define DEFAULT_COLOUR_VALUE             0x00FFFFFF
#define PAIRING_COLOUR_VALUE             0x000DEBEB
#define CELEBRATION_EXCITED_COLOUR_VALUE 0x00FFDBF4
#define SUCCESS_COLOUR_VALUE             0x005CD242
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

void gimi_pb_leds_on(void);
void gimi_pb_leds_off(void);

void gimi_pb_leds_ramp_up(uint32_t colour_code);
void gimi_pb_leds_constant(uint32_t colour_code);
void gimi_pb_leds_hold(void);
void gimi_pb_leds_ramp_down(uint32_t colour_code);

void set_red_duty(uint32_t duty);
void set_green_duty(uint32_t duty);
void set_blue_duty(uint32_t duty);

#endif // __GIMI_PB_LEDS_H
