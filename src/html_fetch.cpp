//
// Gimi Piggy, Copyright 2026.
//

#include <WiFi.h>
#include <HTTPClient.h>

#include "html_fetch.h"

// GitHub raw content URL
const char* url = "https://raw.githubusercontent.com/dostr/dostr.github.io/master/gimipiggyhome.html";

void fetchHTML() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    Serial.println("\nFetching HTML from GitHub...");
    Serial.print("URL: ");
    Serial.println(url);
    
    // Begin HTTP connection
    http.begin(url);
    
    // Set timeout
    http.setTimeout(10000);
    
    // Send GET request
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      
      if (httpResponseCode == HTTP_CODE_OK) {
        // Get the response payload
        String payload = http.getString();
        
        Serial.println("\n=== HTML CONTENT START ===");
        Serial.println(payload);
        Serial.println("=== HTML CONTENT END ===");
        
        Serial.print("\nContent length: ");
        Serial.print(payload.length());
        Serial.println(" bytes");
      } else {
        Serial.print("HTTP error: ");
        Serial.println(httpResponseCode);
      }
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }
    
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}

