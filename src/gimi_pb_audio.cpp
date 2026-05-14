//
// Copyright 2026.
//

#include <Arduino.h>
#include <Audio.h>

#include "gimi_pb_audio.h"

#define SD_CS          5
#define SD_MOSI       23
#define SD_MISO       19
#define SD_SCK        18

Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);

void gimi_pb_audio_setup() {

  Serial.println("STARTUP");

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  SPI.setFrequency(1000000);
  SD.begin(SD_CS);

  Serial.println("Setting Audio Volume");

  audio.setVolume(21); // 0...21
}

void gimi_pb_audio_play()
{

    Serial.println("About to play audio");

    audio.connecttoFS(SD, "goal_achieved_celebration_boosted_300.mp3");

    while (audio.isRunning())
      audio.loop();

}

