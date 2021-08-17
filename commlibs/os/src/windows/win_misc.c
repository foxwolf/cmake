/**
 * @file win_misc.c
 * @brief win misc
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-02-01
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#include <time.h>  
#include <winsock2.h>

int gettimeofday(struct timeval *tv, struct timezone *tz) {
  time_t t;
  t = time(NULL);
  SYSTEMTIME st;
  GetLocalTime(&st);

  tv->tv_sec = (long)t;
  tv->tv_usec = st.wMilliseconds * 1000;

  return 0;
}


struct tm *localtime_r(const time_t *timep, struct tm *result) {
  localtime_s(result, timep);
  return result;
}

