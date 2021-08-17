/**
 * @file sd_log_base.h
 * @brief abstract interface
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-01-27
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#ifndef _COMM_SD_LOG_BASE_H_
#define _COMM_SD_LOG_BASE_H_

typedef  struct _sd_log_base_s sd_log_base_t;

struct _sd_log_base_s {
  void *obj;

  void (*destruct)(sd_log_base_t *obj);

  int32_t (*open)(sd_log_base_t *thiz, const char *path, const char *mode, long level, int maxlen, long msglen);
  int32_t (*close)(sd_log_base_t *thiz);

  int32_t (*save)(sd_log_base_t *thiz);

  int32_t (*set_level)(sd_log_base_t *thiz, unsigned char u8MinLevel);
  int32_t (*msg)(sd_log_base_t *thiz, unsigned char u8MinLevel, char *fmt, ...);
};

#ifdef __cplusplus
extern "C" {
#endif

sd_log_base_t *construct_log_t(void);

#ifdef __cplusplus
}
#endif

#endif
