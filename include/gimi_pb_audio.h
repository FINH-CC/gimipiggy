//
// Copyright 2026.
//

#ifndef __GIMI_PB_AUDIO_H
#define __GIMI_PB_AUDIO_H

#define GIMI_PB_FILE_AUDIO_CLIP_0 "bootup.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_1 "charging.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_2 "error.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_3 "goal_achieved_celebration.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_4 "low_battery.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_5 "money_added_coins_clinking.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_6 "notification_coin_clink.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_7 "notification_excited_oink.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_8 "notification_oink.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_9 "success.wav"

/*#define GIMI_PB_FILE_AUDIO_CLIP_0 "bootup_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_1 "charging_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_2 "error_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_3 "goal_achieved_celebration_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_4 "low_battery_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_5 "money_added_coins_clinking_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_6 "notification_coin_clink_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_7 "notification_excited_oink_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_8 "notification_oink_ffmpeg.wav"
#define GIMI_PB_FILE_AUDIO_CLIP_9 "success_ffmpeg.wav"*/

void gimi_pb_audio_setup(void);
void gimi_pb_audio_play(void);

#endif // __GIMI_PB_AUDIO_H
