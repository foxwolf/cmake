/**
 * @file sd_stack.h
 * @brief stack
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-02-26
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#ifndef _COMM_SD_STACK_H_
#define _COMM_SD_STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct stack_node {
  void *pdata;
};

#define   STACK_MODE_STATIC   1
#define   STACK_MODE_DYNAMIC  2

struct stack_head {
  int mode;
  int max;
  int num;
  int size;
  void *stack;
};

/*
 * stack init, malloc memory, the stack head and the node.
 */
static struct stack_head *stack_init(int max, int size) {
  struct stack_head *ptr;

  if (max <= 0) return NULL;
  
  if (size == 0) {
    ptr = (struct stack_head *)calloc(1, sizeof(struct stack_head) + sizeof(struct stack_node) * max);
    if (ptr == NULL) {
      return NULL;
    }
    //memset(ptr, 0, (sizeof(struct stack_head) + sizeof(struct stack_node) * max));

    ptr->mode = STACK_MODE_DYNAMIC;
    ptr->num = 0;
    ptr->size = 0;
    ptr->max = max;
    ptr->stack = (struct stack_node *)((long)ptr + sizeof(struct stack_head));
  } else {
    ptr = (struct stack_head *)calloc(1, sizeof(struct stack_head) + size * max);
    if (ptr == NULL) {
      return NULL;
    }
    //memset(ptr, 0, (sizeof(struct stack_head) + size * max));

    ptr->mode = STACK_MODE_STATIC;
    ptr->num = 0;
    ptr->size = size;
    ptr->max = max;
    ptr->stack = (struct stack_node *)((long)ptr + sizeof(struct stack_head));
  }

  return ptr;
}

/*
 * free the momory.
 */
static void stack_uninit(struct stack_head *ptr) {
  if (ptr == NULL) return;
  
  free(ptr);

  return;
}

static void stack_push(struct stack_head *ptr, void *p) {
  if (ptr->num < ptr->max) {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      ((struct stack_node *)ptr->stack)[ptr->num].pdata = p;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      memcpy(((char *)ptr->stack + ptr->size * ptr->num), p, ptr->size);
    }

    ptr->num++;
  }
}

static void *stack_pop(struct stack_head *ptr) {
  int num ;
  void *p = NULL;

  if (ptr->num > 0) {
    ptr->num--;
    num = ptr->num - 1;
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[num].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = (char *)ptr->stack + ptr->size * num;
    }
  }

  return p;
}

static void *stack_cur(struct stack_head *ptr) {
  int cur = ptr->num - 1;
  void *p = NULL;

  if (ptr->num <= 0) {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[0].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = ptr->stack;
    }
  } else {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[cur].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = (char *)ptr->stack + ptr->size * cur;
    }
  }

  return p;
}

static void *stack_upper(struct stack_head *ptr) {
  int num = ptr->num - 2;
  void *p = NULL;

  if (num <= 0) {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[0].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = ptr->stack;
    }
  } else {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[num].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = (char *)ptr->stack + ptr->size * num;
    }
  }

  return p;
}

static void *stack_step(struct stack_head *ptr, int step) {
  int num = ptr->num - step;
  void *p = NULL;

  if (num <= 0) {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[0].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = ptr->stack;
    }
  } else {
    if (ptr->mode == STACK_MODE_DYNAMIC) {
      p = ((struct stack_node *)ptr->stack)[num].pdata ;
    } else if (ptr->mode == STACK_MODE_STATIC) {
      p = (char *)ptr->stack + ptr->size * num;
    }
  }

  return p;
}

#ifdef __cplusplus
}
#endif
#endif
