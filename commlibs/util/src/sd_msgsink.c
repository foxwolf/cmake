/**
 * @file sd_listlib.c
 * @brief list lib
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-06-18
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sd_typedef.h"
#include "sd_msgsink.h"

typedef struct sd_msgsink_s sd_msgsink_t, *sd_msgsink_p;

struct sd_msgsink_s {
  void    *obj;

  void (*destruct)(sd_msgsink_t *obj);
  int (*post_message)(sd_msgsink_t *obj, long id, long message_id, long message_info);
};

sd_msgsink_t *construct_msgsink_t(int (* message_recv)(long, long, long, long), long userdata);

typedef struct sd_msg_obj_s {
  long userdata;
  int (*message_recv)(long userdata, long id, long message_id, long message_info);
} sd_msg_obj_t;

static int post_message(sd_msgsink_t *obj, long id, long message_id, long message_info) {
  sd_msg_obj_t *msg_obj = NULL;
  sd_msgsink_t   *msg_link = obj;
  if (msg_link) {
    msg_obj = (sd_msg_obj_t *)msg_link->obj;
  }
  if (msg_obj)
    return msg_obj->message_recv(msg_obj->userdata, id, message_id, message_info);
  return 0;
}

static void msg_slink_destruct(sd_msgsink_t *msg_link) {
  if (msg_link) {
    if (msg_link->obj)
      free(msg_link->obj);
    free(msg_link);
  }
}

sd_msgsink_t *construct_msgsink_t(int (* message_recv)(long, long, long, long), long userdata) {
  sd_msgsink_t   *msg_link;
  sd_msg_obj_t *msg_obj;

  if (NULL == (msg_link = (sd_msgsink_t *)calloc(1, sizeof(sd_msgsink_t)))) {
    return NULL;
  }

  if (NULL == (msg_obj = (sd_msg_obj_t *)calloc(1, sizeof(sd_msg_obj_t)))) {
    free(msg_link);
    return NULL;
  }

  msg_obj->userdata = userdata;
  msg_obj->message_recv = message_recv;

  msg_link->obj = msg_obj;
  msg_link->destruct = msg_slink_destruct;
  msg_link->post_message = post_message;

  return (msg_link);
}

HMSGSINK    sdCreateMsgSink(int (* message_recv)(long, long, long, long), long userdata) {
  return (HMSGSINK)construct_msgsink_t(message_recv, userdata);
}

int       sdPostMsg(HMSGSINK  hMsgSink, long id, long message_id, long message_info) {
  sd_msgsink_p  p = (sd_msgsink_p)hMsgSink;

  if (NULL == p)
    return 0;

  return p->post_message(p, id, message_id, message_info);
}

void      sdCloseMsgSink(HMSGSINK hMsgSink) {
  sd_msgsink_p  p = (sd_msgsink_p)hMsgSink;
  return p->destruct(p);
}

