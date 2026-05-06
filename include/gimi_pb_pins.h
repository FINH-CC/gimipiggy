//
// Copyright 2026.
//

#ifndef __GIMI_PB_PINS_H
#define __GIMI_PB_PINS_H

#define GIMI_PB_GPIO_00 0 // Boot Button, also used to clear WiFi settings, when device woken by push button. Press and HOLD puush button, whilst PRESS-AND_RELEASING this button.
#define GIMI_PB_GPIO_01 1
#define GIMI_PB_GPIO_02 2
#define GIMI_PB_GPIO_03 3
#define GIMI_PB_GPIO_04 4 // Red Channel RGB LED.
#define GIMI_PB_GPIO_05 5
#define GIMI_PB_GPIO_06 6
#define GIMI_PB_GPIO_07 7
#define GIMI_PB_GPIO_08 8
#define GIMI_PB_GPIO_09 9

#define GIMI_PB_GPIO_10 10
#define GIMI_PB_GPIO_11 11
#define GIMI_PB_GPIO_12 12
#define GIMI_PB_GPIO_13 13
#define GIMI_PB_GPIO_14 14
#define GIMI_PB_GPIO_15 15
#define GIMI_PB_GPIO_16 16 // Green Channel RGB LED.
#define GIMI_PB_GPIO_17 17 // Blue Channel RGB LED.
#define GIMI_PB_GPIO_18 18
#define GIMI_PB_GPIO_19 19

#define GIMI_PB_GPIO_20 20
#define GIMI_PB_GPIO_21 21 // Backlight. Also used for Printer power-enable.
#define GIMI_PB_GPIO_22 22 // Printer Rx.
#define GIMI_PB_GPIO_23 23
#define GIMI_PB_GPIO_24 24
#define GIMI_PB_GPIO_25 25
#define GIMI_PB_GPIO_26 26
#define GIMI_PB_GPIO_27 27 // Printer Tx.
#define GIMI_PB_GPIO_28 28
#define GIMI_PB_GPIO_29 29

#define GIMI_PB_GPIO_30 30
#define GIMI_PB_GPIO_31 31
#define GIMI_PB_GPIO_32 32
#define GIMI_PB_GPIO_33 33
#define GIMI_PB_GPIO_34 34
#define GIMI_PB_GPIO_35 35 // Push Button, EARS. Used to initiate printing, to initiate WPS, dependent upon the current device mode. Requires external 10K pull-up resistor on ESP32-2432S028 hardware.
#define GIMI_PB_GPIO_36 36
#define GIMI_PB_GPIO_37 37
#define GIMI_PB_GPIO_38 38
#define GIMI_PB_GPIO_39 39

#endif // __GIMI_PB_PINS_H
