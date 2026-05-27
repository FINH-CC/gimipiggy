//
// Copyright 2026.
//

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_printer.h"
#include "gimi_pb_qr204.h"
#include "gimi_pb_server.h"

String etag_array[GIMI_PB_RECEIPTS_TOTAL];
String url_array[GIMI_PB_RECEIPTS_TOTAL];

//uint32_t current_url = 0;
String current_etag = GIMI_PB_RECEIPT_NULL_ETAG;

bool new_file_available = false; // True when a receipt, loaded into the print buffer, differs by ETag to the previous download.
uint32_t new_file_type = GIMI_PB_RECEIPT_TYPE_WELCOME;

uint8_t* fileBuffer   = nullptr;
size_t   fileSize     = 0;

void gimi_pb_bin_file_setup(void) {

    // Set up URL array;

    url_array[0] = GIMI_PB_RECEIPT_WELCOME_URL;
    url_array[1] = GIMI_PB_RECEIPT_DEFAULT_URL;
    url_array[2] = GIMI_PB_RECEIPT_SAVINGS_URL;
    url_array[3] = GIMI_PB_RECEIPT_CELEBRATION_URL;
    url_array[4] = GIMI_PB_RECEIPT_REMINDER_URL;
    url_array[5] = GIMI_PB_RECEIPT_NOTIFICATION_URL;

    // Clear the ETag array;
    etag_array[0] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[1] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[2] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[3] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[4] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[5] = GIMI_PB_RECEIPT_NULL_ETAG;

    current_etag = GIMI_PB_RECEIPT_NULL_ETAG;

    new_file_available = false;
    new_file_type = GIMI_PB_RECEIPT_TYPE_WELCOME;

    fileBuffer   = nullptr;
    fileSize     = 0;
}

void gimi_pb_bin_file_timer_initiated_update() {

    Serial.printf("Binary file update, about to search for new ETag.\n");

    for (uint32_t i = 0; i < GIMI_PB_RECEIPTS_TOTAL; i++) {

        new_file_available = gimi_pb_bin_fetch_and_check_etag_by_receipt_ordinal(i);

        if (new_file_available) {

            Serial.printf("New receipt available to print of type %d\n", i);  
            new_file_type = i;          
            return;
        }
    }
}

void gimi_pb_bin_file_button_initiated_print(void) {

    Serial.printf("Binary file print.\n");

    if (new_file_available == true) {
        Serial.printf("Binary file print - about to fetch and print new receipt.\n");
        gimi_pb_bin_fetch_and_print_receipt_by_ordinal(new_file_type);
    } else {
        Serial.printf("Binary file print - about to fetch and print welcome receipt.\n");
        gimi_pb_bin_fetch_and_print_receipt_by_ordinal(GIMI_PB_RECEIPT_TYPE_WELCOME);
    }
 }

bool gimi_pb_get_bin_new_file_available(void) {
    return new_file_available;
}

uint32_t gimi_pb_get_file_type(void) {
    return new_file_type;
}

size_t gimi_pb_get_bin_file_size(void) {
    return fileSize;
}

uint8_t* gimi_pb_get_bin_file_buffer(void) {
    return fileBuffer;
}

void gimi_pb_set_bin_file_printed(void) {

    new_file_available = false;
    Serial.printf("Receipt printed\n");            
}

// Downloads Receipt by ordinal, and compares the new ETag with the ETag of the previous downloaded version (if one exists).
// Returns true if a NEW receipt is found.

bool gimi_pb_bin_fetch_and_check_etag_by_receipt_ordinal(uint32_t url_number) {

    HTTPClient http;
    http.begin(url_array[url_number].c_str());

    // Optional: set timeouts (ms)
    http.setTimeout(10000);
    http.setConnectTimeout(5000);

    // Define the headers to collect
    const char* headerKeys[] = {"ETag"};
    const size_t headerKeysCount = 1;

    // Configure HTTPClient to collect the specified headers
    http.collectHeaders(headerKeys, headerKeysCount);

    // Does not appear to be evalauted on GitHub.
    //http.addHeader("If-None-Match", "fbb7c88732d1190ed812ea84ffcd845b61b2f38ecec01bc158be16801df33c6d");
    //http.addHeader("User-Agent", "ESP32-HTTPClient/1.0");

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[HTTP] GET failed, code: %d\n", httpCode);
        http.end();
        return false;
    }

    // Request successful, print the collected headers
    Serial.printf("HTTP Response Code: %d\n", httpCode);
    for (size_t i = 0; i < headerKeysCount; i++) {
        String headerValue = http.header(headerKeys[i]);
        Serial.printf("Header %s: %s\n", headerKeys[i], headerValue.c_str());

        if (strcmp(headerKeys[i], "ETag") == 0) {
            current_etag = headerValue;
            Serial.printf("Current ETag  %s: \n", current_etag.c_str());
        }
    }

    // Check if the ETag for the newly downloaded file has changed since the previous download.
    Serial.printf("Previous ETag  %s: \n", etag_array[url_number].c_str());

    if (etag_array[url_number] != current_etag) {

        etag_array[url_number] = current_etag;
        Serial.printf("New printable content available on URL # %d.\n", url_number);

        return true;

    } else {

        Serial.printf("No new content available.\n");
        return false;
    }

    int contentLength = http.getSize();   // -1 if chunked / unknown
}

// Downloads Receipt by ordinal, and print it.
// Returns true if printable content downloaded and sent to printer.
// (no mechanism available to check whther content has actually been successfully printed)

bool gimi_pb_bin_fetch_and_print_receipt_by_ordinal(uint32_t url_number) {

    Serial.printf("Binary file print - about to fetch and print receipt by ordinal.\n");

    fileSize   = 0;

    HTTPClient http;
    http.begin(url_array[url_number].c_str());

    // Optional: set timeouts (ms)
    http.setTimeout(10000);
    http.setConnectTimeout(5000);

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[HTTP] GET failed, code: %d\n", httpCode);
        http.end();
        return false;
    }

    int contentLength = http.getSize();   // -1 if chunked / unknown
    WiFiClient* stream = http.getStreamPtr();

    // ── Path A: known size ──────────────────────────────────────────────
    if (contentLength > 0) {
        Serial.printf("Binary file print - fetching known size of %d bytes.\n", contentLength);
        fileBuffer = (uint8_t*) heap_caps_malloc(contentLength, MALLOC_CAP_SPIRAM); // Explicity use PSRAM, rather than malloc(contentLength);
        if (!fileBuffer) {
            Serial.println("malloc failed — not enough heap");
            http.end();
            return false;
        }

        size_t bytesRead = 0;
        uint32_t timeout = millis();

        while (http.connected() && bytesRead < (size_t)contentLength) {
            size_t available = stream->available();
            if (available) {
                size_t chunk = stream->readBytes(
                    fileBuffer + bytesRead,
                    min(available, (size_t)(contentLength - bytesRead))
                );
                bytesRead += chunk;
                timeout = millis();  // reset watchdog on progress
            }
            if (millis() - timeout > 5000) {
                Serial.println("Stream timeout");
                free(fileBuffer);
                fileBuffer = nullptr;
                http.end();
                return false;
            }
            yield();  // feed the ESP32 watchdog
        }

        fileSize = bytesRead;
        Serial.printf("Downloaded %u bytes\n", fileSize);

    // ── Path B: unknown / chunked size ─────────────────────────────────
    } else {
        Serial.printf("Binary file print - fetching unknown size.\n");
        // Grow a dynamic buffer in chunks
        const size_t CHUNK = 512;
        size_t allocated = CHUNK;
        fileBuffer = (uint8_t*) heap_caps_malloc(allocated, MALLOC_CAP_SPIRAM); // Explicity use PSRAM, rather than malloc(allocated);
        if (!fileBuffer) { http.end(); return false; }

        fileSize = 0;
        uint8_t tmp[CHUNK];

        uint32_t timeout = millis();
        while (http.connected()) {
            size_t available = stream->available();
            if (available) {
                size_t n = stream->readBytes(tmp, min(available, CHUNK));
                // Grow buffer if needed
                if (fileSize + n > allocated) {
                    allocated += max(n, CHUNK);
                    uint8_t* resized = (uint8_t*) heap_caps_realloc(fileBuffer, allocated, MALLOC_CAP_SPIRAM); // Explicity use PSRAM, rather than realloc(fileBuffer, allocated);
                    if (!resized) {
                        free(fileBuffer); fileBuffer = nullptr;
                        http.end(); return false;
                    }
                    fileBuffer = resized;
                }
                memcpy(fileBuffer + fileSize, tmp, n);
                fileSize += n;
                timeout = millis();
            } else if (millis() - timeout > 3000) {
                break;  // assume EOF
            }
            yield();
        }
        Serial.printf("Streamed %u bytes (chunked)\n", fileSize);
    }

    http.end();

    // fileBuffer now holds the raw bytes — do whatever you need:
    Serial.printf("First 4 bytes: %02X %02X %02X %02X\n",
        fileBuffer[0], fileBuffer[1], fileBuffer[2], fileBuffer[3]);

    Serial.printf("About to print receipt from URL # %d.\n", url_number);

    gimi_pb_printer_begin();
    gimi_pb_printer_print_binary();
    gimi_pb_printer_end();

    Serial.printf("Printing complete for receipt URL # %d.\n", url_number);

    free(fileBuffer);
    fileBuffer = nullptr;
    fileSize   = 0;

    return true;
}

