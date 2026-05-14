//
// Copyright 2026.
//

#ifndef __GIMI_PB_BIN_FETCH_H
#define __GIMI_PB_BIN_FETCH_H

// Initialisation.
void gimi_pb_bin_file_setup(void);

// Timer triggered update.
void gimi_pb_bin_file_update(void);

// Button triggered printing support.
bool gimi_pb_get_bin_file_available(void);
size_t gimi_pb_get_bin_file_size(void);
uint8_t* gimi_pb_get_bin_file_buffer(void);
void gimi_pb_set_bin_file_printed(void);

// Button triggered reload of default file.
bool gimi_pb_bin_file_default_get(void);

// Internal to module. Returns true if a new receipt has been found on the server.
bool gimi_pb_bin_fetch_new(uint32_t url_number);

#endif // __GIMI_PB_BIN_FETCH_H
