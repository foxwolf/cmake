/**
 * @file os_windows.h
 * @brief os windows header
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-12-03
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */
#ifndef _OS_OS_WINDOWS_H_
#define _OS_OS_WINDOWS_H_

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <locale.h>
#include <intrin.h>
#include <io.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "winsock2.h"
#include <WS2tcpip.h>
#include <winbase.h>
#include <Winsock2.h>
#include <time.h>
#include <inttypes.h>
#include <conio.h>
#include <math.h>


#define strncasecmp _strnicmp
#define strcasecmp  _stricmp

/**
 * Beginning with the UCRT in Visual Studio 2015 and Windows 10, snprintf is no longer
 * identical to _snprintf. The snprintf function behavior is now C99 standard compliant.
 * @see https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/snprintf-snprintf-snprintf-l-snwprintf-snwprintf-l?f1url=%3FappId%3DDev16IDEF1%26l%3DZH-CN%26k%3Dk(STDIO%252Fsnprintf);k(snprintf);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue&view=msvc-160
 */
#define snprintf _snprintf
#define socklen_t int
#define in_addr_t uint32_t


#define SD_OS_FUNC_SOCKET
#define SD_OS_FUNC_SOCKET_SETSOCKETOPT
#define SD_OS_FUNC_SOCKET_INET

#ifdef __cplusplus
extern "C" {
#endif

#define sdSend(sockfd, buf, len, flags) send((SOCKET)sockfd, buf, len, flags)
#define sdSendto(sockfd, buf, len, flags, dest_addr, addrlen) sendto((SOCKET)sockfd, buf, len, flags, dest_addr, addrlen)
#define sdWriteSocket(fd, buf, len) send((SOCKET)fd, buf, len, 0)
#define sdReadSocket(fd, buf, len) recv((SOCKET)fd, buf, len, 0)
#define sdCloseSocketNoCheck(fd) closesocket((SOCKET)fd)
#define sdCloseSocket(fd) closesocket((SOCKET)fd)
typedef SOCKET eventfd_t;

#ifdef __cplusplus
}
#endif
#endif
