//
// Copyright 2026.
//

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "gimi_pb_bin_fetch.h"
#include "gimi_pb_qr204.h"

const char* URL = "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin";

//
// The system uses a static file buffer of 64K.
//
static uint8_t fileBuffer[STATIC_BINARY_FILE_ALLOCATION];
size_t   fileSize     = 0;

bool gimi_pb_receipt_download(const char* url) {

    fileSize   = 0;

    HTTPClient http;
    http.begin(url);

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
    if (contentLength > 0 && contentLength <= STATIC_BINARY_FILE_ALLOCATION) {

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
                http.end();
                return false;
            }
            yield();  // feed the ESP32 watchdog
        }

        fileSize = bytesRead;
        Serial.printf("Downloaded %u bytes\n", fileSize);

    // ── Path B: unknown / chunked size ─────────────────────────────────
    } else {

        const size_t CHUNK = 512;
        size_t allocated = CHUNK;

        fileSize = 0;
        uint8_t tmp[CHUNK];

        uint32_t timeout = millis();
        while (http.connected()) {
            size_t available = stream->available();
            if (available) {
                size_t n = stream->readBytes(tmp, min(available, CHUNK));

                if (fileSize + n <= STATIC_BINARY_FILE_ALLOCATION) {
                    memcpy(fileBuffer + fileSize, tmp, n);
                } else {
                    return false;
                }

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
    return true;
}

void gimi_pb_receipt_setup() {

    if (gimi_pb_receipt_download(URL)) {
        // fileBuffer now holds the raw bytes — do whatever you need:
        Serial.printf("First 4 bytes: %02X %02X %02X %02X\n",
            fileBuffer[0], fileBuffer[1], fileBuffer[2], fileBuffer[3]);

    }
}

void gimi_pb_receipt_update() {}

size_t gimi_pb_receipt_get_filesize(void) {
    return fileSize;
}

uint8_t* gimi_pb_receipt_get_buffer(void) {
    return fileBuffer;
}
