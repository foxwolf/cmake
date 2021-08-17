/**
 * @file os_semaphore.h
 * @brief semaphore
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-12-15
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */
#ifndef _SD_OS_SEMAPHORE_H_
#define _SD_OS_SEMAPHORE_H_

#ifndef SEM_VALUE_MAX
  #define SEM_VALUE_MAX        0x7FFFU                                          /**< Maximum value of a sem_t. */
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void    *HEVENT;
HEVENT _sdCreateEvent();
void _sdSetEvent(HEVENT hEnt);
int _sdWaitForSingleObject(HEVENT hEnt, uint32_t timeout_ms);
void _sdCloseEvent(HEVENT hEnt);

#ifdef __cplusplus
}
#endif

#endif
