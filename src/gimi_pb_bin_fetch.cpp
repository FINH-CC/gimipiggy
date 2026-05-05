//
// Copyright 2026.
//

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_qr204.h"
#include "gimi_pb_server.h"

String etag_array[GIMI_PB_RECEIPTS_TOTAL];
String url_array[GIMI_PB_RECEIPTS_TOTAL];

uint32_t current_url = 0;
String current_etag = GIMI_PB_RECEIPT_NULL_ETAG;

bool new_file_available = false;
uint8_t* fileBuffer   = nullptr;
size_t   fileSize     = 0;

bool buffer_allocated = false;

void gimi_pb_bin_file_setup(void) {

    // Set up URL array;

    url_array[0] = GIMI_PB_RECEIPT_0_URL;
    url_array[1] = GIMI_PB_RECEIPT_1_URL;
    url_array[2] = GIMI_PB_RECEIPT_2_URL;
    url_array[3] = GIMI_PB_RECEIPT_3_URL;
    url_array[4] = GIMI_PB_RECEIPT_4_URL;
    url_array[5] = GIMI_PB_RECEIPT_5_URL;

    // Clear the ETag array;
    etag_array[0] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[1] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[2] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[3] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[4] = GIMI_PB_RECEIPT_NULL_ETAG;
    etag_array[5] = GIMI_PB_RECEIPT_NULL_ETAG;

    current_url = 0;
    current_etag = GIMI_PB_RECEIPT_NULL_ETAG;

    new_file_available = false;
    fileBuffer   = nullptr;
    fileSize     = 0;

    buffer_allocated = false;
}

// Search through all URL's to see if any new content is available.
// Stop on first instance of new content.
void gimi_pb_bin_file_update() {

    for (uint32_t i = 0; i < GIMI_PB_RECEIPTS_TOTAL; i++) {

        new_file_available = gimi_pb_bin_fetch_new(i);

        if (new_file_available) {

            Serial.printf("Receipt available to print\n");            
            return;
        }
    }
}

bool gimi_pb_get_bin_file_available(void) {
    return new_file_available;
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
// Heap allocation is persists until this function is called again.
// Manages own heap allocation.

bool gimi_pb_bin_fetch_new(uint32_t url_number) {

    // If the file buffer has been used on previous calls, free it now.
    if (buffer_allocated == true) {
        free(fileBuffer);
        fileBuffer = nullptr;
        fileSize   = 0;
        buffer_allocated = false;
    }

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

    int contentLength = http.getSize();   // -1 if chunked / unknown
    WiFiClient* stream = http.getStreamPtr();

    // ── Path A: known size ──────────────────────────────────────────────
    if (contentLength > 0) {
        fileBuffer = (uint8_t*) malloc(contentLength);
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
        // Grow a dynamic buffer in chunks
        const size_t CHUNK = 512;
        size_t allocated = CHUNK;
        fileBuffer = (uint8_t*) malloc(allocated);
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
                    uint8_t* resized = (uint8_t*) realloc(fileBuffer, allocated);
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

    buffer_allocated = true;

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

}
