/**
 * @file sd_listlib.c
 * @brief list lib
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-05-19
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#include "../include/sd_listlib.h"

void        list_cont_init(list_t *list, void (* delete_node)(node_t *node)) {
  list->num = 0;
  list->delete_node = delete_node;
  INIT_LIST_HEAD(&list->head);
}

void        list_cont_clear(list_t *list) {
  node_t *node;

  node = list_cont_pop(list);
  while (node) {
    if (node && list->delete_node)
      list->delete_node(node);
    node = list_cont_pop(list);
  }
}

void        list_cont_erase(list_t *list, node_t *node) {
  if (list_empty(&list->head))
    return;

  list_del_init(&node->head);
  list->num--;
  if (list->delete_node)
    list->delete_node(node);
}

void        list_cont_push(list_t *list, node_t *node) {
  list_add_tail(&node->head, &list->head);
  list->num++;
}

node_t     *list_cont_pop(list_t *list) {
  node_t *node;
  struct list_head *entry = NULL;

  if (list_empty(&list->head))
    return (NULL);
  entry = list->head.next;
  list_del_init(entry);
  list->num--;

  node = list_entry(entry, node_t, head);

  return (node);
}

int         list_cont_size(list_t *list) {
  return list->num;
}

int         list_cont_empty(list_t *list) {
  return (list_empty(&list->head));
}

node_t     *list_cont_begin(list_t *list) {
  node_t   *node = NULL;
  struct list_head     *entry = NULL;

  if (list_empty(&list->head)) {
    printf("33333333333\n");
    return (NULL);
  }

  entry = list->head.next;
  node = list_entry(entry, node_t, head);
  printf("33333333333 node=%p\n",node);

  return (node);
}

node_t     *list_cont_next(node_t *node) {
  node_t *next = NULL;
  struct list_head     *entry = NULL;

  entry = node->head.next;
  next = list_entry(entry, node_t, head);

  return (next);
}

node_t     *list_cont_prev(node_t *node) {
  node_t *next = NULL;
  struct list_head *entry = NULL;

  entry = node->head.prev;
  next = list_entry(entry, node_t, head);

  return (next);
}

// end 是最后一个之后，前闭后开
node_t     *list_cont_end(list_t *list) {
  node_t *node = NULL;
  struct list_head *entry = NULL;

  entry = &list->head;
  node = list_entry(entry, node_t, head);

  return (node);
}

int         list_cont_trv(list_t *list, int(* trv)(node_t *, void *), void *par) {
  struct list_head *entry = NULL, *next = NULL;
  node_t *node = NULL;

  list_for_each_safe(entry, next, &list->head) {
    node = list_entry(entry, node_t, head);
    if (trv(node, par) == 0)
      return RET_OK;
  }

  return RET_FAIL;
}

int         list_cont_ptr_check(list_t *list, node_t *ptr) {
  struct list_head *entry = NULL, *next = NULL;
  node_t *node = NULL;

  if (list == (list_t *)ptr)
    return RET_OK;

  list_for_each_safe(entry, next, &list->head) {
    node = list_entry(entry, node_t, head);
    if (node == ptr)
      return RET_OK;
  }

  return RET_FAIL;
}
