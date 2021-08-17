/**
 * @file os_misc.h
 * @brief os misc header
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-12-03
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */
#ifndef _OS_OS_MISC_H_
#define _OS_OS_MISC_H_

#define FD_INITIALIZER  ((int32_t)-1)
#define INVALID_SOCKET FD_INITIALIZER

#ifdef __cplusplus
extern "C" {
#endif

/*
char *strncpy(char *dest, const char *src, size_t n);
Warning:
  If there is no null byte among the first n bytes of src, the string placed in dest will not be null-termi‐nated.
*/
#define sdstrncpy(dst, src, size)   \
  do {                             \
    strncpy((dst), (src), (size)); \
    (dst)[(size)-1] = 0;           \
  } while (0);

int64_t sdGetSelfPthreadId();
int kbhit(void);

#ifdef __cplusplus
}
#endif
#endif
