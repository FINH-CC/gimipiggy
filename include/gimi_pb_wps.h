//
// Copyright 2026.
//

#ifndef __GIMI_PB_WPS_H
#define __GIMI_PB_WPS_H

void gimi_pb_wps_setup(void);
void gimi_pb_wps_update(void);

void gimi_pb_wps_init_config();
void gimi_pb_wifi_event(WiFiEvent_t event, arduino_event_info_t info);

#endif // __GIMI_PB_WPS_H
