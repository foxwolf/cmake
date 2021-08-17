/**
 * @file os_socket.c
 * @brief socket
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-11-27
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"
#include "os_socket.h"

#ifndef SD_OS_FUNC_SOCKET

int32_t sdSetSockNonblock(SOCKET sock, bool isOn) {
  int32_t flags = 0;
  if ((flags = fcntl(sock, F_GETFL, 0)) < 0) {
    printf("fcntl(F_GETFL) error: %d (%s)\n", errno, strerror(errno));
    return RET_FAIL;
  }

  if (isOn)
    flags |= O_NONBLOCK;
  else
    flags &= ~O_NONBLOCK;

  if ((flags = fcntl(sock, F_SETFL, flags)) < 0) {
    printf("fcntl(F_SETFL) error: %d (%s)\n", errno, strerror(errno));
    return RET_FAIL;
  }

  return RET_OK;
}

#endif

#ifndef SD_OS_FUNC_SOCKET_SETSOCKETOPT

int32_t sdSetSockOpt(SOCKET socketfd, int32_t level, int32_t optname, void *optval, int32_t optlen) {
  return setsockopt(socketfd, level, optname, optval, (socklen_t)optlen);
}

#endif

#ifndef SD_OS_FUNC_SOCKET_INET

uint32_t sdInetAddr(const char *ipAddr) {
  return inet_addr(ipAddr);
}

const char *sdInetNtoa(struct in_addr ipInt) {
  return inet_ntoa(ipInt);
}

#endif

