//
// Copyright 2026.
//

#ifndef __GIMI_PB_PRINTER_H
#define __GIMI_PB_PRINTER_H

// ******************************************************************** //
//                                                                      //
// NOTE THAT GOOJPRT QR204 CANNOT PRINT UPSIDE DOWN TEXT WITH println() //
//                                                                      //
// TO ENSURE CORRECT OPERATION, PRECEDE EVERY println() with            //
// upsideDownPrinting(0);                                               //
// IF THERE IS UNCERTAINTY IF upsideDownPrinting is ON                  //
//                                                                      //
// ******************************************************************** //




void piggy_printer_setup(void);
void piggy_printer_update(void);




void printMemoryStats(const char* tag);
void setGlobalUpsideDown(bool enable);

void piggy_experimental_printer_setup();
void piggy_experimental_printer_loop();

// Parse and execute serial commands
void handleSerialCommand(String cmd);

// Set print darkness (% from 50 to 205%) and break delay (µs)
// This controls how dark the print appears and timing between print operations
void setDarknessAndDelay(uint8_t densityPercent, uint16_t breakDelayUs);

void printQRCode(String data); // Print QR Code with specified data. Uses the printer's built-in QR code generation capability.
void parseBarcode(String params); // Parse barcode command and call appropriate printing function.
void printBarcode_CODE128(String data); // Print CODE128 barcode - variable length alphanumeric.
void printBarcode_UPCA(String data); // Print UPC-A barcode - requires exactly 11 digits.
void printBarcode_EAN13(String data); // Print EAN13 barcode - requires exactly 12 digits.

// Printer control functions - these send ESC/POS commands to control printer behavior
void feedLines(int n); // Print and feed n lines - creates vertical spacing.
void upsideDownPrinting(int n); // Turn upside-down printing mode on/off.
void underlineMode(int n); // Set underline mode - 0=off, 1=1dot thick, 2=2dots thick.
void align(int n); // Set text justification - 0=left, 1=center, 2=right.
void inverseMode(int n); // Turn inverse (white on black) mode on/off.

void printBitmap(uint8_t* imageData, int width, int height); // Print bitmap image using standard GS v command - optimized for memory usage.

void createTestPattern(uint8_t* buffer, int width, int height, int pattern); // Create test pattern in bitmap buffer - useful for printer testing.
void printTestPattern(int width, int height, int pattern); // Print a test pattern - useful for testing printer functionality.

// Alternative bitmap printing method using DC2 * command
// This method prints line by line to avoid spacing issues
void printBitmapDC2_Method(const unsigned char* progmemData, int width, int height);

// Helper function: Rotate a 1bpp bitmap by 180 degrees
// Returns a newly allocated buffer that caller must free()
// Used for upside-down printing when globalUpsideDown is enabled
uint8_t* rotateBitmap1bpp_180(const uint8_t* src, int width, int height);

// Main bitmap printing method using GS v command with chunking
// This is the primary method for printing large images efficiently
void printBitmapGS_Method(const unsigned char* progmemData, int width, int height);

// Helper function to print a named bitmap from the availableImages array
void printNamedBitmap(String imageName);

// Demo function: Print next image in sequence (triggered by Key1)
void demoPrintNextImage();

// Demo function: Print comprehensive demo page (triggered by Key2)
// Shows all printer capabilities in one printout
void demoPrintAllFormats();

#endif // __GIMI_PB_PRINTER_H

