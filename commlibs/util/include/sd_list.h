/**
 * @file sd_list.c
 * @brief base list struct
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-05-19
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#ifndef _UTIL_SD_LIST_H_
#define _UTIL_SD_LIST_H_

#include <stdio.h>
#include "sd_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

struct list_head {
	struct list_head *next, *prev;
};
typedef struct list_head LISTHEAD,*PLISTHEAD;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD_(name) struct list_head name = LIST_HEAD_INIT(name)

//static inline void INIT_LIST_HEAD(struct list_head *list)
static
#ifdef  _WIN32
#else
inline
#endif
void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/*
 * Insert a pnew entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static
#ifdef  _WIN32
#else
inline
#endif
void __list_add(struct list_head *pnew,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = pnew;
	pnew->next = next;
	pnew->prev = prev;
	prev->next = pnew;
}

/**
 * list_add - add a pnew entry
 * @pnew: pnew entry to be added
 * @head: list head to add it after
 *
 * Insert a pnew entry after the specified head.
 * This is good for implementing stacks.
 */
static
#ifdef  _WIN32
#else
inline
#endif
void list_add(struct list_head *pnew, struct list_head *head)
{
	__list_add(pnew, head, head->next);
}

/**
 * list_add_tail - add a pnew entry
 * @pnew: pnew entry to be added
 * @head: list head to add it before
 *
 * Insert a pnew entry before the specified head.
 * This is useful for implementing queues.
 */
static
#ifdef  _WIN32
#else
inline
#endif
void list_add_tail(struct list_head *pnew, struct list_head *head)
{
	__list_add(pnew, head->prev, head);
}


/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static
#ifdef  _WIN32
#else
inline
#endif
void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static
#ifdef  _WIN32
#else
inline
#endif
void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}


/**
 * list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
 static
#ifdef  _WIN32
#else
inline
#endif
void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static
#ifdef  _WIN32
#else
inline
#endif
void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}

/**
 * list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
 static
#ifdef  _WIN32
#else
inline
#endif
void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
 
static
#ifdef  _WIN32
#else
inline
#endif
int list_empty(const struct list_head *head)
{
	return head->next == head;
}

static
#ifdef  _WIN32
#else
inline
#endif
int list_notempty(const struct list_head *head)
{
	return head->next != head;
}

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)

/**
 * list_for_each_entry	-	iterate over list of given type
 * @entry:	the type * to use as a loop counter.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


#ifdef __cplusplus
}
#endif

#endif
