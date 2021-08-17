/**
 * @file osFreeRTOS.h
 * @brief FreeRTOS
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-08-23
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#ifndef _SD_OS_OSFREERTOS_H_
#define _SD_OS_OSFREERTOS_H_

#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <float.h>
#include <libgen.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <termios.h>
#include <wchar.h>
#include <wordexp.h>
#include <wctype.h>
#include <inttypes.h>
#include <sys/resource.h>
#include <math.h>
#include <stdatomic.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "tcpip_adapter.h"
#include "esp_event_loop.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_eth.h"
#include "esp_wifi.h"
#include "esp_private/wifi.h"
#include "driver/gpio.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "esp_spiffs.h"

#include "driver/can.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_ping.h"
#include "ping/ping.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "lwip/ip4.h"

#include "sd_hal_ethernet.h"
#include "sd_hal_misc.h"
#include "sd_hal_forward.h"

#define atomic_load_8(ptr) atomic_load(ptr)
#define atomic_load_16(ptr) atomic_load(ptr)
#define atomic_load_32(ptr) atomic_load(ptr)
#define atomic_load_64(ptr) atomic_load(ptr)
#define atomic_load_ptr(ptr) atomic_load(ptr)

#define atomic_store_8(ptr, val) atomic_store(ptr, val)
#define atomic_store_16(ptr, val) atomic_store(ptr, val)
#define atomic_store_32(ptr, val) atomic_store(ptr, val)
#define atomic_store_64(ptr, val) atomic_store(ptr, val)
#define atomic_store_ptr(ptr, val) atomic_store(ptr, val)

#define atomic_exchange_8(ptr, val) atomic_exchange(ptr, val)
#define atomic_exchange_16(ptr, val) atomic_exchange(ptr, val)
#define atomic_exchange_32(ptr, val) atomic_exchange(ptr, val)
#define atomic_exchange_64(ptr, val) atomic_exchange(ptr, val)
#define atomic_exchange_ptr(ptr, val) atomic_exchange(ptr, val)

#define atomic_val_compare_exchange_8 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_16 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_32 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_64 __sync_val_compare_and_swap
#define atomic_val_compare_exchange_ptr __sync_val_compare_and_swap

//#define atomic_add_fetch_8(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_add_fetch_16(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_add_fetch_32(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_add_fetch_64(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_add_fetch_ptr(ptr, val) __atomic_add_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_add_fetch_8(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_add_fetch_16(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_add_fetch_32(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_add_fetch_64(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_add_fetch_ptr(ptr, val) atomic_fetch_add(ptr, val)

#define atomic_fetch_add_8(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_fetch_add_16(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_fetch_add_32(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_fetch_add_64(ptr, val) atomic_fetch_add(ptr, val)
#define atomic_fetch_add_ptr(ptr, val) atomic_fetch_add(ptr, val)

//#define atomic_sub_fetch_8(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_sub_fetch_16(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_sub_fetch_32(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_sub_fetch_64(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_sub_fetch_ptr(ptr, val) __atomic_sub_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_sub_fetch_8(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_sub_fetch_16(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_sub_fetch_32(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_sub_fetch_64(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_sub_fetch_ptr(ptr, val) atomic_fetch_sub(ptr, val)

#define atomic_fetch_sub_8(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_fetch_sub_16(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_fetch_sub_32(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_fetch_sub_64(ptr, val) atomic_fetch_sub(ptr, val)
#define atomic_fetch_sub_ptr(ptr, val) atomic_fetch_sub(ptr, val)

//#define atomic_and_fetch_8(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_and_fetch_16(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_and_fetch_32(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_and_fetch_64(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_and_fetch_ptr(ptr, val) __atomic_and_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_and_fetch_8(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_and_fetch_16(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_and_fetch_32(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_and_fetch_64(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_and_fetch_ptr(ptr, val) atomic_fetch_and(ptr, val)

#define atomic_fetch_and_8(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_fetch_and_16(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_fetch_and_32(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_fetch_and_64(ptr, val) atomic_fetch_and(ptr, val)
#define atomic_fetch_and_ptr(ptr, val) atomic_fetch_and(ptr, val)

//#define atomic_or_fetch_8(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_or_fetch_16(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_or_fetch_32(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_or_fetch_64(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_or_fetch_ptr(ptr, val) __atomic_or_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_or_fetch_8(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_or_fetch_16(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_or_fetch_32(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_or_fetch_64(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_or_fetch_ptr(ptr, val) atomic_fetch_or(ptr, val)

#define atomic_fetch_or_8(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_fetch_or_16(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_fetch_or_32(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_fetch_or_64(ptr, val) atomic_fetch_or(ptr, val)
#define atomic_fetch_or_ptr(ptr, val) atomic_fetch_or(ptr, val)

//#define atomic_xor_fetch_8(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_xor_fetch_16(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_xor_fetch_32(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_xor_fetch_64(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)
//#define atomic_xor_fetch_ptr(ptr, val) __atomic_xor_fetch((ptr), (val), __ATOMIC_SEQ_CST)

#define atomic_xor_fetch_8(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_xor_fetch_16(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_xor_fetch_32(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_xor_fetch_64(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_xor_fetch_ptr(ptr, val) atomic_fetch_xor(ptr, val)

#define atomic_fetch_xor_8(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_fetch_xor_16(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_fetch_xor_32(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_fetch_xor_64(ptr, val) atomic_fetch_xor(ptr, val)
#define atomic_fetch_xor_ptr(ptr, val) atomic_fetch_xor(ptr, val)


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
