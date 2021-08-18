/**
 * @file win_socket.c
 * @brief windows socket
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-12-03
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */
#include "sd_os.h"
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <ws2def.h>
#include <tchar.h>

#define SOL_TCP                  0x1234

void sdWinSockInit() {
  static char flag = 0;
  if (flag == 0) {
    WORD    wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(1, 1);
    if (WSAStartup(wVersionRequested, &wsaData) == 0) {
      flag = 1;
    }
  }
}

int32_t sdSetSockNonblock(SOCKET sock, bool isOn) {
  u_long mode;
  if (isOn) {
    mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
  } else {
    mode = 0;
    ioctlsocket(sock, FIONBIO, &mode);
  }
  return 0;
}

void sdBlockSIGPIPE() {}

int32_t sdSetSockOpt(SOCKET socketfd, int32_t level, int32_t optname, void *optval, int32_t optlen) {
  if (level == SOL_SOCKET && optname == TCP_KEEPCNT) {
    return 0;
  }

  if (level == SOL_TCP && optname == TCP_KEEPIDLE) {
    return 0;
  }

  if (level == SOL_TCP && optname == TCP_KEEPINTVL) {
    return 0;
  }

  if (level == SOL_TCP && optname == TCP_KEEPCNT) {
    return 0;
  }

  return setsockopt(socketfd, level, optname, optval, optlen);
}

#ifdef SD_OS_FUNC_SOCKET_INET

uint32_t sdInetAddr(const char *ipAddr) {
  uint32_t value;
  int32_t ret = inet_pton(AF_INET, ipAddr, &value);
  if (ret <= 0) {
    return INADDR_NONE;
  } else {
    return value;
  }
}

const char *sdInetNtoa(struct in_addr ipInt) {
  // not thread safe, only for debug usage while print log
  static char tmpDstStr[16];
  return inet_ntop(AF_INET, &ipInt, tmpDstStr, INET6_ADDRSTRLEN);
}

#endif


int set_ipaddr(unsigned long iaddr, char *iface) {
  return RET_OK;
}

int set_netmask(unsigned long mask, char *iface) {
  return RET_OK;
}

int set_defgw(unsigned long gw, char *iface) {
  return RET_OK;
}

int get_ipaddr(uint32_t *p, char *iface) {
  return RET_OK;
}

int get_netmask(unsigned long *p, char *iface) {
  return RET_OK;
}

int get_gateway(unsigned long *p, char *netname) {
  return RET_OK;
}

int get_macaddr(char *cp, char *iface) {
  return RET_OK;
}

int del_rt(unsigned long dest, unsigned long gw, unsigned long mask, char *iface) {
  return RET_OK;
}

int add_rt(unsigned long dest, unsigned long gw, unsigned long mask, char *iface) {
  return RET_OK;
}

