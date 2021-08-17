/*
 * FreeRTOS POSIX V1.2.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file FreeRTOS_POSIX_semaphore.c
 * @brief Implementation of functions in semaphore.h
 */
#include "os_freertos_esp32.h"
#include "os_semaphore.h"
#include "sd_typedef.h"

typedef SemaphoreHandle_t sem_t;
#define INFINITE            (~0)  // Infinite timeout


int sem_destroy(sem_t *sem);
int sem_getvalue(sem_t *sem,
                 int *sval);
int sem_init(sem_t *sem,
             int pshared,
             unsigned value);
int sem_post(sem_t *sem);
int sem_timedwait(sem_t *sem,
                  const struct timespec *abstime);
int sem_trywait(sem_t *sem);
int sem_wait(sem_t *sem);

int sem_destroy(sem_t *sem) {

  /* Free the resources in use by the semaphore. */
  vSemaphoreDelete(*sem);
  *sem = NULL;

  return 0;
}

int sem_init(sem_t *sem,
              int pshared,
              unsigned value) {
  int iStatus = RET_FAIL;

  if (sem) {
    *sem = xSemaphoreCreateCounting(SEM_VALUE_MAX, value);
    if ((*sem) != NULL) {
      iStatus = RET_OK;
    }
  }

  return iStatus;
}

int sem_post(sem_t *sem) {
  if (xSemaphoreGive(*sem) == pdTRUE)
    return RET_OK;

  return RET_FAIL;
}

int sem_timedwait(sem_t *sem,
                   const struct timespec *abstime) {
  TickType_t timeout;

  if (abstime != NULL) {
    timeout = abstime->tv_sec * 1000 / portTICK_PERIOD_MS;
    timeout += abstime->tv_nsec / 1000000 / portTICK_PERIOD_MS;
  } else {
    timeout = portMAX_DELAY;
  }

  if (xSemaphoreTake(*sem, timeout) == pdTRUE) return RET_OK;

  errno = ETIMEDOUT;

  return RET_FAIL;
}

 int sem_trywait(sem_t *sem) {
  int iStatus = 0;

  /* Setting an absolute timeout of 0 (i.e. in the past) will cause sem_timedwait
   * to not block. */
  struct timespec xTimeout = { 0 };

  iStatus = sem_timedwait(sem, &xTimeout);

  /* POSIX specifies that this function should set errno to EAGAIN and not
   * ETIMEDOUT. */
  if ((iStatus == -1) && (errno == ETIMEDOUT)) {
    errno = EAGAIN;
  }

  return iStatus;
}

int sem_wait(sem_t *sem) {
  return sem_timedwait(sem, NULL);
}

HEVENT _sdCreateEvent() {
  sem_t *sem = NULL;

  sem = (sem_t *)calloc(1, sizeof(sem_t));
  if (sem == NULL) return NULL;
  sem_init(sem, 0, 0);
  return (HEVENT)sem;
}

void _sdSetEvent(HEVENT hEnt) {
  sem_t *sem = (sem_t *)hEnt;

  sem_post(sem);
}

int _sdWaitForSingleObject(HEVENT hEnt, uint32_t timeout_ms) {
  sem_t *sem = (sem_t *)hEnt;

  int ret = RET_OK;

    if (INFINITE == timeout_ms) {
        if (xSemaphoreTake(*sem, portMAX_DELAY) != pdTRUE) {
            ret = RET_FAIL;
        }
    } else {
        if (xSemaphoreTake(*sem, timeout_ms / portTICK_PERIOD_MS) != pdTRUE) {
            ret = RET_FAIL;
        }
    }

    return ret;
}

void _sdCloseEvent(HEVENT hEnt) {
  sem_t *sem = (sem_t *)hEnt;

  sem_destroy(sem);
  if (sem != NULL) {
    free(sem);
    sem = NULL;
  }
}

