/**
 * @file os_socket.h
 * @brief os socket header
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-11-28
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */
#ifndef _OS_OS_SOCKET_H_
#define _OS_OS_SOCKET_H_

typedef int32_t SOCKET;
#define	SA	struct sockaddr

#ifdef __cplusplus
extern "C" {
#endif

#define sdSend(sockfd, buf, len, flags) send(sockfd, buf, len, flags)
#define sdSendto(sockfd, buf, len, flags, dest_addr, addrlen) sendto(sockfd, buf, len, flags, dest_addr, addrlen)
#define sdReadSocket(fd, buf, len) read(fd, buf, len)
#define sdWriteSocket(fd, buf, len) write(fd, buf, len)
#define sdCloseSocket(x) close(x)

int32_t sdSetSockNonblock(SOCKET sock, bool isOn);
int32_t sdSetSockOpt(SOCKET socketfd, int32_t level, int32_t optname, void *optval, int32_t optlen);

uint32_t sdInetAddr(const char *ipAddr);
const char *sdInetNtoa(struct in_addr ipInt);

#ifdef __cplusplus
}
#endif

#endif
