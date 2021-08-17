/**
 * @file os_def.h
 * @brief os define
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-01-29
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#ifndef _COMM_OS_DEF_H_
#define _COMM_OS_DEF_H_

#ifndef STDERR_FILENO
#define STDERR_FILENO (2)
#endif

#define FD_VALID(x) ((x) > STDERR_FILENO)
#define FD_INITIALIZER  ((int32_t)-1)

#define RET_OK    0
#define RET_FAIL  -1


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cpluslus
}
#endif
#endif
