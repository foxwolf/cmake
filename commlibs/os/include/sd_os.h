/**
 * @file sd_os.h
 * @brief os header
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-08-23
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */
#ifndef _OS_INCLUDE_OS_H_
#define _OS_INCLUDE_OS_H_

#if defined(_SD_ESP32_PLATFORM)
#include "os_freertos_esp32.h"
#endif

#if defined(_SD_LINUX_32) || defined(_SD_LINUX_64)
#include "os_linux.h"
#endif

#if defined(_SD_WINDOWS_64) || defined(_SD_WINDOWS_32)
#include "os_windows.h"
#endif

#include "os_def.h"
#include "os_socket.h"
#include "os_misc.h"
#include "os_semaphore.h"
#include "os_file.h"

void osInit();
void osReboot();

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif
