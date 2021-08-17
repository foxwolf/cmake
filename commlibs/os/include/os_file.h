/**
 * @file os_file.h
 * @brief os file header
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-01-29
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#ifndef _COMM_OS_FILE_H_
#define _COMM_OS_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

int64_t sdReadImp(int32_t fd, void *buf, int64_t count);
int64_t sdWriteImp(int32_t fd, void *buf, int64_t n);

#define sdRead(fd, buf, count) sdReadImp(fd, buf, count)
#define sdWrite(fd, buf, count) sdWriteImp(fd, buf, count)
#define sdLSeek(fd, offset, whence) sdLSeekImp(fd, offset, whence)
#define sdClose(fd)      \
  {                        \
    if (FD_VALID(fd)) {    \
      close(fd);           \
      fd = FD_INITIALIZER; \
    }                      \
  }

#ifdef __cplusplus
}
#endif
#endif
