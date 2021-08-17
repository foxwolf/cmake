/**
 * @file sd_log.c
 * @brief log
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-01-27
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"
#include "sd_log.h"
#include "sd_log_base.h"

#define MAX_LOGLINE_SIZE              (1000)
#define MAX_LOGLINE_BUFFER_SIZE       (MAX_LOGLINE_SIZE + 10)
#define MAX_LOGLINE_CONTENT_SIZE      (MAX_LOGLINE_SIZE - 100)
#define MAX_LOGLINE_DUMP_SIZE         (65 * 1024)
#define MAX_LOGLINE_DUMP_BUFFER_SIZE  (MAX_LOGLINE_DUMP_SIZE + 10)
#define MAX_LOGLINE_DUMP_CONTENT_SIZE (MAX_LOGLINE_DUMP_SIZE - 100)

typedef struct _sd_log_s {
  bool        isInit;

  uint8_t     ucMinLevel;
  uint32_t    lenMsg;
  char        *pszFileName;
  char        *pMsg;

  FILE        *pstruFileHandle;
  uint32_t    u32MaxSize;        /*日志文件的最大容量,以KB为单位*/

  HMUTEX      lock;
} sd_log_t;

sd_log_base_t *construct_log_t() {
  sd_log_base_t *p = NULL;
  sd_log_t *pObj = NULL;

  /*Allocate the object space*/
  if (NULL == (p = (sd_log_base_t *)malloc(sizeof(sd_log_base_t)))) {
    return NULL;
  } else {
    memset(p, '\0', sizeof(sd_log_base_t));
  }

  if (NULL == (pObj = (sd_log_t *)malloc(sizeof(sd_log_t)))) {
    free(p);
    return NULL;
  } else {
    memset(pObj, '\0', sizeof(sd_log_t));
  }

  /*init mutex*/
  pObj->lock = sdCreateMutex();
  if (pObj->lock == NULL) {
    //WARN(MODULE_NAME, "call %s error\n", "pthread_mutex_init");
    free(p);
    return NULL;
  }

  pObj->isInit = false;

  /*set the new ways*/
  p->obj = NULL; //pObj;
  p->open = NULL; //log_open;
  p->close = NULL; //log_destruct;
  p->save = NULL; //log_save;
  p->destruct  = NULL; //log_destruct;
  p->set_level = NULL; //log_set_level;
  p->msg = NULL; //log_msg;

  return p;
}

HLOG        sdLogCreate() {
  sd_log_base_t *p = construct_log_t();
  return (HLOG)p;
}

int32_t     sdLogOpen(HLOG hlog, const char *path, const char *mode, long level, int maxlen, long msglen) {
  int32_t ret = RET_OK;

  return ret;
}

void        sdLogClose(HLOG hlog) {
}

int32_t     sdLogSave(HLOG hlog) {
  int32_t ret = RET_OK;

  return ret;
}

int32_t     sdLogSetLevel(HLOG hlog, unsigned char u8MinLevel) {
  int32_t ret = RET_OK;

  return ret;
}

int32_t     sdLogMsg(HLOG hlog, unsigned char u8MinLelel, char *fmt, ...) {
  int32_t ret = RET_OK;

  return ret;
}

void        sdLogRelease(HLOG hlog) {
}


void     sdPrintLog(const char *flags, int32_t dflag, const char *format, ...) {
  va_list        argpointer;
  char           buffer[MAX_LOGLINE_BUFFER_SIZE] = { 0 };
  int32_t        len;

#if defined(_SD_ESP32_PLATFORM)
  len = sprintf(buffer, "%ld ", sdGetTimeMs());
#else
  struct timeval timeSecs;
  time_t         curTime;
  struct tm      Tm, *ptm;

  gettimeofday(&timeSecs, NULL);
  curTime = timeSecs.tv_sec;
  ptm = localtime_r(&curTime, &Tm);
  len = sprintf(buffer, "%02d/%02d %02d:%02d:%02d.%06d 0x%08" PRIx64 " ", ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour,
                ptm->tm_min, ptm->tm_sec, (int32_t)timeSecs.tv_usec, sdGetSelfPthreadId());
#endif

  len += sprintf(buffer + len, "%s", flags);

  va_start(argpointer, format);
  int32_t writeLen = vsnprintf(buffer + len, MAX_LOGLINE_CONTENT_SIZE, format, argpointer);
  if (writeLen <= 0) {
    char tmp[MAX_LOGLINE_BUFFER_SIZE] = {0};
    writeLen = vsnprintf(tmp, MAX_LOGLINE_CONTENT_SIZE, format, argpointer);
    if (writeLen <= 0) {
      printf("Log Module call vsnprintf error!\n");
      return;
    }
    strncpy(buffer + len, tmp, MAX_LOGLINE_CONTENT_SIZE);
    len += writeLen;
  } else if (writeLen >= MAX_LOGLINE_CONTENT_SIZE) {
    len += MAX_LOGLINE_CONTENT_SIZE;
  } else {
    len += writeLen;
  }
  va_end(argpointer);

  if (len > MAX_LOGLINE_SIZE) len = MAX_LOGLINE_SIZE;

  buffer[len++] = '\n';
  buffer[len] = 0;

  sdWrite(1, buffer, (uint32_t)len);
}

