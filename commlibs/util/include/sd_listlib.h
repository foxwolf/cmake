/**
 * @file sd_listlib.h
 * @brief  list lib
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-05-19
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#ifndef _UTIL_SD_LISTLIB_H_
#define _UTIL_SD_LISTLIB_H_

#include "sd_typedef.h"
#include "sd_list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  _node_s {
  struct  list_head head;
  void   *obj;
}node_t;

typedef struct  _list_s {
  struct  list_head head;
  int     num;
  void    (*delete_node)(node_t *node);
}list_t;

void        list_cont_init(list_t *list, void (*delete_node)(node_t *node));
void        list_cont_clear(list_t *list);
void        list_cont_erase(list_t *list, node_t *node);
void        list_cont_push(list_t *list, node_t *node);
node_t     *list_cont_pop(list_t *list);
int         list_cont_size(list_t *list);
int         list_cont_empty(list_t *list);
node_t     *list_cont_begin(list_t *list);
node_t     *list_cont_next(node_t *node);
node_t     *list_cont_prev(node_t *node);
node_t     *list_cont_end(list_t *list);
int         list_cont_trv(list_t *list, int(* trv)(node_t *, void *), void *par);
int         list_cont_ptr_check(list_t *list, node_t *ptr);

#ifdef __cplusplus
}
#endif

#endif
