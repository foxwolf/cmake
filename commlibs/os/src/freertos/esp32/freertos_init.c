/**
 * @file freertos_init.c
 * @brief freertos
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-12-29
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"

#if 0
static void initUTC() {
  struct timeval tv;
  struct tm lt;
  int ret;

  lt.tm_sec   = 1;
  lt.tm_min   = 0;
  lt.tm_hour  = 0;
  lt.tm_mday  = 1;
  lt.tm_mon   = 1 - 1;
  lt.tm_year  = 1970 - 1900;
  lt.tm_wday  = 0;
  lt.tm_yday  = 0;
  lt.tm_isdst = 0;

  tv.tv_sec = mktime(&lt);
  tv.tv_usec = 0;

  ret = settimeofday(&tv, NULL);
  if (-1 == ret) {
    printf("%s error: %s\n", __FUNCTION__, strerror(errno));
  }
}
#endif

void osInit() {
  tcpip_adapter_init();

  ESP_ERROR_CHECK(esp_event_loop_create_default());

  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  //initUTC();
}

void osReboot() {
  esp_restart();
}

