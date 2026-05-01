//
// Copyright 2026.
//

#ifndef __GIMI_PB_BIN_FETCH_H
#define __GIMI_PB_BIN_FETCH_H

void gimi_pb_receipt_setup(void);
void gimi_pb_receipt_update(void);

bool gimi_pb_receipt_download(const char* url);

size_t gimi_pb_receipt_get_filesize(void);
uint8_t* gimi_pb_receipt_get_buffer(void);

#endif // __GIMI_PB_BIN_FETCH_H
