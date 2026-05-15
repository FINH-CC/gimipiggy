//
// Copyright 2026.
//

#ifndef __GIMI_PB_AUDIO_H
#define __GIMI_PB_AUDIO_H

#define MAX_AUDIO_FILENAME_LENGTH 256
#define GIMI_PB_SOUNDS_TOTAL 10

enum gimi_pb_sound { 
    SOUND_BOOTUP = 0,
    SOUND_CHARGING,
    SOUND_ERROR,
    SOUND_CELEBRATION,
    SOUND_LOW_BATTERY,
    SOUND_MONEY_ADDED,
    SOUND_COIN_CLINK,
    SOUND_EXCITED_OINK,
    SOUND_NOTIFICATION_OINK,
    SOUND_SUCCESS
};

#define GIMI_PB_FILE_AUDIO_CLIP_BOOTUP            "bootup.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_CHARGING          "charging.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_ERROR             "error.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_CELEBRATION       "goal_achieved_celebration_boosted_300.mp3" //"goal_achieved_celebration.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_LOW_BATTERY       "low_battery.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_MONEY_ADDED       "money_added_coins_clinking.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_COIN_CLINK        "notification_coin_clink.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_EXCITED_OINK      "notification_excited_oink.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_NOTIFICATION_OINK "notification_oink.mp3"
#define GIMI_PB_FILE_AUDIO_CLIP_SUCCESS           "success.mp3"

void gimi_pb_audio_setup(void);
void gimi_pb_audio_play(uint32_t sound_id);

#endif // __GIMI_PB_AUDIO_H
