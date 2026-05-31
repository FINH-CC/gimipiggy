//
// Copyright 2026.
//

#ifndef __GIMI_PB_BIN_FETCH_H
#define __GIMI_PB_BIN_FETCH_H

// Initialisation.
void gimi_pb_bin_file_setup(void);

// Timer triggered update.
void gimi_pb_bin_file_timer_initiated_update(void);

// Button triggered printing.
void gimi_pb_bin_file_button_initiated_print(void);

// Button triggered printing support.
bool gimi_pb_get_bin_new_file_available(void);
uint32_t gimi_pb_get_file_type(void);
size_t gimi_pb_get_bin_file_size(void);
uint8_t* gimi_pb_get_bin_file_buffer(void);
void gimi_pb_set_bin_file_printed(void);

// Internal to module.
bool gimi_pb_bin_fetch_and_check_etag_by_receipt_ordinal(uint32_t url_number);
bool gimi_pb_bin_fetch_and_print_receipt_by_ordinal_uses_malloc(uint32_t url_number);
bool gimi_pb_bin_fetch_and_print_receipt_by_ordinal(uint32_t url_number);

#endif // __GIMI_PB_BIN_FETCH_H
