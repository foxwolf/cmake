/**
 * @file os_file.c
 * @brief os file oper
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-01-29
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"
#include "os_file.h"

int64_t sdReadImp(int32_t fd, void *buf, int64_t count) {
  int64_t leftbytes = count;
  int64_t readbytes;
  char   *tbuf = (char *)buf;

  while (leftbytes > 0) {
    readbytes = read(fd, (void *)tbuf, (uint32_t)leftbytes);
    if (readbytes < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        return RET_FAIL;
      }
    } else if (readbytes == 0) {
      return (int64_t)(count - leftbytes);
    }

    leftbytes -= readbytes;
    tbuf += readbytes;
  }

  return count;
}

int64_t sdWriteImp(int32_t fd, void *buf, int64_t n) {
  int64_t leftbytes = n;
  int64_t nwritten = 0;
  char   *tbuf = (char *)buf;

  while (leftbytes > 0) {
    nwritten = write(fd, (void *)tbuf, (uint32_t)leftbytes);
    if (nwritten < 0) {
      if (errno == EINTR) {
        continue;
      }
      return RET_FAIL;
    }
    leftbytes -= nwritten;
    tbuf += nwritten;
  }

  return n;
}

int64_t sdLSeekImp(int32_t fd, int64_t offset, int32_t whence) {
  return (int64_t)lseek(fd, (long)offset, whence);
}

