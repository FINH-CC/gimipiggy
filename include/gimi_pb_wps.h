//
// Copyright 2026.
//

#ifndef __GIMI_PB_WPS_H
#define __GIMI_PB_WPS_H

void piggy_wps_setup(void);
void piggy_wps_update(void);

void piggy_wps_init_config();
void piggy_wifi_event(WiFiEvent_t event, arduino_event_info_t info);

#endif // __GIMI_PB_WPS_H
