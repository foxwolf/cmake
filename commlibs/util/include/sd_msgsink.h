/**
 * @file sd_msgsink.h
 * @brief msgsink
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-06-18
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#ifndef _SD_UTIL_SD_MSGSINK_H_
#define _SD_UTIL_SD_MSGSINK_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef HANDLE  HMSGSINK;
typedef int (sd_msg_sink)(long userdata, long id, long msgid, long info);

HMSGSINK    sdCreateMsgSink(int (* message_recv)(long, long, long, long), long userdata);
int         sdPostMsg(HMSGSINK  hMsgSink, long id, long message_id, long message_info);
void        sdCloseMsgSink(HMSGSINK hMsgSink);

#ifdef __cplusplus
}
#endif

#endif
