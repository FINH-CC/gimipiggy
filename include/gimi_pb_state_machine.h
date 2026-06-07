//
// Copyright 2026.
//

#ifndef __GIMI_PB_STATE_MACHINE_H
#define __GIMI_PB_STATE_MACHINE_H

void gimi_pb_state_machine_setup(void);

void gimi_pb_state_machine_handle_timer(void);
void gimi_pb_state_machine_handle_button(void);

void gimi_pb_state_machine_play_sequence_by_receipt_type(uint32_t receipt_type, bool with_sound);
void gimi_pb_state_machine_play_light_and_sound_sequence(uint32_t light, uint32_t sound);
void gimi_pb_state_machine_play_light_only_sequence(uint32_t light);
void gimi_pb_state_machine_play_sound_only_sequence(uint32_t sound);

void gimi_pb_state_machine_printer_pre_sequence(uint32_t light);
void gimi_pb_state_machine_printer_post_sequence(uint32_t light, uint32_t sound);

void gimi_pb_state_machine_set_button_pressed(void);
void gimi_pb_state_machine_clear_button_pressed(void);
bool gimi_pb_state_machine_get_button_pressed(void);

#endif // __GIMI_PB_STATE_MACHINE_H
