/**
 * @file linux_misc.c
 * @brief misc
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-12-24
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"

void osInit() {
}

int32_t sdFtruncate(int32_t fd, int64_t length) {
  return ftruncate(fd, length);
}

