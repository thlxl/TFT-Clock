//
// Created by xl on 2024/4/26.
//

#ifndef SHIZHONG_SZ_WIFI_H
#define SHIZHONG_SZ_WIFI_H

#define CONFIG_EXAMPLE_CONNECT_WIFI 1
#define CONFIG_EXAMPLE_WIFI_CONN_MAX_RETRY 6
#define CONFIG_EXAMPLE_WIFI_SSID "TP-LINK_13A1"
#define CONFIG_EXAMPLE_WIFI_PASSWORD "AI3D1112"
#define CONFIG_EXAMPLE_WIFI_SCAN_METHOD_ALL_CHANNEL 1
#define CONFIG_EXAMPLE_WIFI_SCAN_RSSI_THRESHOLD -127
#define CONFIG_EXAMPLE_WIFI_AUTH_OPEN 1
#define CONFIG_EXAMPLE_WIFI_CONNECT_AP_BY_SIGNAL 1

#define CONFIG_SNTP_TIME_SERVER "pool.ntp.org"

struct tm sz_get_sntptime(void);
void sz_wifi_connect(void);
void sz_wifi_disconnect(void);
void sz_wifi_init(void);

#endif //SHIZHONG_SZ_WIFI_H
