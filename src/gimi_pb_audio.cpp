//
// Copyright 2026.
//

#include <Arduino.h>
#include <Audio.h>

#include "gimi_pb_audio.h"
#include "gimi_pb_pins.h"

#define SD_CS         GIMI_PB_GPIO_26
#define SD_MOSI       GIMI_PB_GPIO_12
#define SD_MISO       GIMI_PB_GPIO_14
#define SD_SCK        GIMI_PB_GPIO_27
#define I2S_BCLK      GIMI_PB_GPIO_32
#define I2S_LRC       GIMI_PB_GPIO_33
#define I2S_DOUT      GIMI_PB_GPIO_25

String gimi_pb_sound_array[GIMI_PB_SOUNDS_TOTAL];


//Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN); // Use for internal DAC on ESP32-2432S028 CYD, and comment out audio.setPinout().
Audio audio; // USe for external DAC, such as MAX98357 as used in the AdaFruit BFF.

void gimi_pb_audio_setup() {

  Serial.println("AUDIO STARTUP");

  // Set up sounds array;

  gimi_pb_sound_array[0] = GIMI_PB_FILE_AUDIO_CLIP_BOOTUP;
  gimi_pb_sound_array[1] = GIMI_PB_FILE_AUDIO_CLIP_CHARGING;
  gimi_pb_sound_array[2] = GIMI_PB_FILE_AUDIO_CLIP_ERROR;
  gimi_pb_sound_array[3] = GIMI_PB_FILE_AUDIO_CLIP_LOW_BATTERY; ;
  gimi_pb_sound_array[4] = GIMI_PB_FILE_AUDIO_CLIP_CELEBRATION;
  gimi_pb_sound_array[5] = GIMI_PB_FILE_AUDIO_CLIP_COIN_CLINK;
  gimi_pb_sound_array[6] = GIMI_PB_FILE_AUDIO_CLIP_EXCITED_OINK; 
  gimi_pb_sound_array[7] = GIMI_PB_FILE_AUDIO_CLIP_MONEY_ADDED;
  gimi_pb_sound_array[8] = GIMI_PB_FILE_AUDIO_CLIP_NOTIFICATION_OINK;
  gimi_pb_sound_array[9] = GIMI_PB_FILE_AUDIO_CLIP_SUCCESS;

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
  SPI.setFrequency(1000000);
  SD.begin(SD_CS);

  Serial.println("Setting Audio Volume");

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT); // Comment out this line if using internal DAC.
  audio.setVolume(21); // 0...21
}

void gimi_pb_audio_play(uint32_t sound_id)
{

    Serial.printf("About to play audio %s\n", gimi_pb_sound_array[sound_id].c_str());

    audio.connecttoFS(SD, gimi_pb_sound_array[sound_id].c_str());

    while (audio.isRunning())
      audio.loop();

}

