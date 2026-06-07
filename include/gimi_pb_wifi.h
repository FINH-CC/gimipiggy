//
// Copyright 2026.
//

#ifndef __GIMI_PB_WIFI_H
#define __GIMI_PB_WIFI_H

bool gimi_pb_wifi_manager_setup(void);
bool gimi_pb_wifi_manager_is_config_mode(void);

void gimi_pb_wifi_manager_reconnect(void);
bool gimi_pb_wifi_manager_is_connected(void);
void gimi_pb_wifi_manager_disconnect(void);

void gimi_pb_wifi_manager_restart_wifi_setup(void);

#endif // __GIMI_PB_WIFI_H
