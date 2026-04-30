//
// Gimi Piggy, Copyright 2026.
//

#ifndef __GIMI_PB_QR204_H
#define __GIMI_PB_QR204_H

// This header describes the characteristics of the GOOJPRT QR204 printer,
// and the restrictions and limits on what can be printed.
//
// The printer uses 58mm thermal paper on a roll.
// However, the printable area is limited to the middle 48mm.
// (i.e. 5 mm unprinted gutters each side)
//
// The print resolution  8 pixels per mm,
// which corresponds to a printable width of 384 pixels.
//
// When the printer is used in graphics mode, a binary file is downloaded,
// where the binary data is packed as 8 pixels per byte.
//
// For simplicity, all graphics must be exactly 384 pixels wide,
// and due to memory constraints in the device,
// the maximum length of any printable binary is limited to 170 mm.
//
// This result in a maximum binary file size of (384 * (170 * 8)) / 8 = 65280,
// which fits inside a 64K static memory allocation.
//
// Printable binaries MUST BE exact multiples of 384 bytes. 

#define PIXELS_PER_MM 8
#define PRINTABLE_WIDTH 48 // In mm.
#define PIXELS_PER_LINE PIXELS_PER_MM * PRINTABLE_WIDTH // 384 pixels.

#define STATIC_BINARY_FILE_ALLOCATION 65536 //131072

#endif // __GIMI_PB_QR204_H
