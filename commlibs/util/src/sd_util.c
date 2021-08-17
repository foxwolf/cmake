/**
 * @file util.c
 * @brief utility
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2020-08-04
 */
/* Copyright (C) 2018-2020, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"
#include "sd_util.h"
#include "sd_typedef.h"
#include "sd_ulog.h"

time_t sdGetTimeMs() {
  struct timespec abs_timeout;

  clock_gettime(CLOCK_MONOTONIC, &abs_timeout);

  return abs_timeout.tv_sec * 1000 + abs_timeout.tv_nsec / 1000000;
}

time_t sdGetTimeS() {
  return sdGetTimeMs() / 1000;
}

bool sdCheckTickTimerMs(time_t *last, uint32_t delta) {
  time_t cur = sdGetTimeMs();
  int64_t d = cur >= *last ? (cur - *last) : -1;

  if ((delta <= 0) || (0 == cur)) {
    *last = cur;
    return FALSE;
  }

  if (-1 == d) {
    return FALSE;
  }

  if (d >= delta) {
    *last = cur;
    return TRUE;
  }

  return FALSE;
}

bool sdCheckTickTimerS(time_t *last, uint32_t delta) {
  time_t cur = sdGetTimeS();
  int64_t d = cur >= *last ? (cur - *last) : -1;

  if ((delta <= 0) || (0 == cur)) {
    *last = cur;
    return FALSE;
  }

  if (-1 == d) {
    return FALSE;
  }

  if (d >= delta) {
    *last = cur;
    return TRUE;
  }

  return FALSE;
}

void sdTaskDelayMs(uint32_t value) {
  TickType_t dealy = pdMS_TO_TICKS(value);
  vTaskDelay(dealy);
}

void sdSleepS(uint32_t time_s) {
  usleep(time_s * 1000000UL);
}

void sdSleepMs(uint32_t time_ms) {
  usleep(time_ms * 1000);
  //ets_delay_us(1000 * time_ms);
}

/* note: The sdGetDateTimeStr() function not be reentrant, so is not thread-safe. */
const char *sdGetDateTimeStr() {
  static char cTimeStr[96];
  time_t tn;
  struct tm *ts, tm;

  time(&tn);
  ts = localtime_r(&tn, &tm);
  snprintf(cTimeStr, sizeof(cTimeStr), "%d-%02d-%02d %02d:%02d:%02d", ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday,
           ts->tm_hour, ts->tm_min, ts->tm_sec);

  return cTimeStr;
}

#if 1
typedef struct _sd_mutex_t {
  int8_t        num;
  pthread_mutex_t *pMutex;
} sd_mutex_t;

HMUTEX sdCreateMutex() {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)calloc(1, sizeof(sd_mutex_t));
  if (NULL == pSdMutex) return NULL;

  pSdMutex->num    = 0;
  pSdMutex->pMutex = (pthread_mutex_t *)calloc(1, sizeof(pthread_mutex_t));
  if (NULL == pSdMutex->pMutex) return NULL;

  if (pthread_mutex_init(pSdMutex->pMutex, NULL)) {
    free(pSdMutex->pMutex);
    free(pSdMutex);
    return NULL;
  }

  return (HMUTEX)pSdMutex;
}

void sdDestroyMutex(HMUTEX thiz) {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)thiz;
  pthread_mutex_destroy(pSdMutex->pMutex);
  free(pSdMutex->pMutex);
  free(pSdMutex);
}

void sdMutexLock(HMUTEX thiz) {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)thiz;

  pSdMutex->num++;
  pthread_mutex_lock(pSdMutex->pMutex);
}

void sdMutexUnLock(HMUTEX thiz) {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)thiz;

  pthread_mutex_unlock(pSdMutex->pMutex);
  pSdMutex->num--;
  if (pSdMutex->num > 0)
    sched_yield(); //出让cpu，使得线程或进程都有机会运行
  else if (pSdMutex->num < 0)
    pSdMutex->num = 0;
}
#else
typedef struct _sd_mutex_t {
  int8_t        num;
  void       *pMutex;
} sd_mutex_t;

HMUTEX sdCreateMutex() {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)calloc(1, sizeof(sd_mutex_t));
  if (NULL == pSdMutex) return NULL;

  pSdMutex->num    = 0;
  pSdMutex->pMutex =  xSemaphoreCreateMutex();
  if (NULL == pSdMutex->pMutex) return NULL;

  return (HMUTEX)pSdMutex;
}

void sdDestroyMutex(HMUTEX thiz) {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)thiz;
  vSemaphoreDelete(pSdMutex->pMutex);
  pSdMutex->pMutex = NULL;
  free(pSdMutex);
  pSdMutex = NULL;
}

void sdMutexLock(HMUTEX thiz) {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)thiz;

  pSdMutex->num++;
  if (xSemaphoreTake(pSdMutex->pMutex, portMAX_DELAY) != pdTRUE) {
    sdError(("xSemaphoreTake error!\n"));
  }
}

void sdMutexUnLock(HMUTEX thiz) {
  sd_mutex_t *pSdMutex = (sd_mutex_t *)thiz;

  xSemaphoreGive(pSdMutex->pMutex);

  pSdMutex->num--;
  if (pSdMutex->num > 0)
    sched_yield(); //出让cpu，使得线程或进程都有机会运行
  else if (pSdMutex->num < 0)
    pSdMutex->num = 0;
}


#endif

int32_t  sdUtilUnitTest() {
  int8_t loop = 0;
  time_t check_time_ms, check_time_s;

  /* 初始化值 */
  sdCheckTickTimerMs(&check_time_ms, 0);
  sdCheckTickTimerS(&check_time_s, 0);

  printf("check_time_ms: %ld; sdGetTimeMs: %ld\n", check_time_ms, sdGetTimeMs());
  printf("check_time_s: %ld; sdCheckTickTimerS: %ld\n", check_time_s, sdGetTimeS());

  while (loop <= 10) { //测试10秒
    if (sdCheckTickTimerMs(&check_time_ms, 2)) {
      printf("sdGetTimeMs: %ld\n", sdGetTimeMs());
      //ESP_LOGI(TAG, "sdGetTimeMs: %d\n", sdGetTimeMs());
    }
    if (sdCheckTickTimerS(&check_time_s, 1)) {
      printf("sdCheckTickTimerS: %ld\n", sdGetTimeS());
      //ESP_LOGI(TAG, "sdCheckTickTimerS: %d\n", sdGetTimeS());
      loop++;
    }
    esp32_feed_wdt();
  }

  uInfo("sdUtilUnitTest Finish!\n")
  taskYIELD(); //通知调度器现在就切换到其它任务

  return 0;
}

uint32_t sdArgsGetOpt(const char *arg, const char *key, char *value, long size) {
  char keyt[25];
  char argt[256];
  char *b, *e;

  if (NULL == arg)
    return RET_FAIL;

  memset(keyt, 0, sizeof(keyt));
  memset(argt, 0, sizeof(argt));
  strcpy(argt, arg);
  if (key[0] != '-') {
    keyt[0] = '-';
    strcpy(keyt + 1, key);
  } else
    strcpy(keyt, key);

  b = strstr(argt, keyt);
  if (b == NULL)
    return RET_FAIL;

  b += strlen(keyt) + 1;
  e = strstr(b, " -");
  if (e)
    e[0] = '\0';
  strcpy(value, b);

  return RET_OK;
}

int32_t sdArgsGetIntOpt(const char *arg, char *key, int32_t defaultval) {
  char retarg[255];
  if (sdArgsGetOpt(arg, key, retarg, 255) != RET_OK)
    return defaultval;
  return atoi(retarg);
}

/*support opt=NULL.*/
int32_t sdArgsSplitOpt(const char *arg, char *opt, long size, char *retarg) {
  char *b;

  if (NULL == arg) return RET_FAIL;

  if (arg[0] == '-') {
    strcpy(retarg, "");
    strcpy(opt, arg);
    return RET_OK;
  }
  b = strstr(arg, " -");
  if (b == NULL) {
    strcpy(retarg, arg);
    if (NULL != opt)
      opt[0] = '\0';
    return RET_OK;
  }
  if (NULL != opt)
    strcpy(opt, b);

  memcpy(retarg, arg, b - arg);
  retarg[b - arg] = '\0';

  return RET_OK;
}

/* start event */
HEVENT sdCreateEvent() {
  return _sdCreateEvent();
}

void sdSetEvent(HEVENT hEnt) {
  _sdSetEvent(hEnt);
}

int sdWaitForSingleObject(HEVENT hEnt, uint32_t timeout_ms) {
  return _sdWaitForSingleObject(hEnt, timeout_ms);
}

void sdDestroyEvent(HEVENT hEnt) {
  _sdCloseEvent(hEnt);
}
//end evevnt*/

/*start msg queue*/
typedef struct {
  int queueSize;
  int dataSize;
  void *dataPtr;
  int idxRd;
  int idxWr;
  int count;
  pthread_mutex_t lock;
  pthread_cond_t  condRd;
  pthread_cond_t  condWr;
} QUEUE;

void *sdCreateQueue(int queueSize, int dataSize) {
  QUEUE *q;
  //  void *dataPtr;
  q = malloc(sizeof(QUEUE));
  if (q == NULL)
    return NULL;
  q->dataPtr = malloc(queueSize * dataSize);
  if (q->dataPtr == NULL) {
    free(q);
    return NULL;
  }
  q->queueSize = queueSize;
  q->dataSize = dataSize;
  q->idxRd = q->idxWr = 0;
  q->count = 0;

  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->condRd, NULL);
  pthread_cond_init(&q->condWr, NULL);

  return q;
}

void sdDestroyQueue(void *handle) {
  QUEUE *q = (QUEUE *)handle;
  pthread_mutex_destroy(&q->lock);
  pthread_cond_destroy(&q->condRd);
  pthread_cond_destroy(&q->condWr);
  free(q->dataPtr);
  free(q);
}

int sdPutQueue(void *handle, const void *data, int timeout_ms) {
  QUEUE *q = (QUEUE *)handle;
  int status = -1;
  struct timeval tv;
  struct timespec ts;

  if (timeout_ms > 0) {
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    while (timeout_ms > 1000) {
      timeout_ms -= 1000;
      ts.tv_sec++;
    }
    ts.tv_nsec = tv.tv_usec * 1000 + timeout_ms * 1000000;
    while (ts.tv_nsec >= 1000000000) {
      ts.tv_nsec -= 1000000000;
      ts.tv_sec++;
    }
  }

  pthread_mutex_lock(&q->lock);

  while (1) {
    if (q->count < q->queueSize) {
      memcpy(q->dataPtr + q->idxWr * q->dataSize, data, q->dataSize);
      q->idxWr++;
      if (q->idxWr >= q->queueSize)
        q->idxWr = 0;
      if (q->count == 0)
        pthread_cond_signal(&q->condRd);
      q->count++;
      status = 0;
      break;
    } else {
      if (timeout_ms == 0)
        break;
      if (timeout_ms < 0)
        status = pthread_cond_wait(&q->condWr, &q->lock);
      else
        status = pthread_cond_timedwait(&q->condWr, &q->lock, &ts);
      if (status != 0) {
        status = -1;
        break;
      }
    }
  }

  pthread_mutex_unlock(&q->lock);

  return status;
}

int sdGetQueue(void *handle, void *data, int timeout_ms) {
  QUEUE *q = (QUEUE *)handle;
  int status = -1;
  struct timeval tv;
  struct timespec ts;

  if (timeout_ms > 0) {
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec;
    while (timeout_ms > 1000) {
      timeout_ms -= 1000;
      ts.tv_sec++;
    }
    ts.tv_nsec = tv.tv_usec * 1000 + timeout_ms * 1000000;
    while (ts.tv_nsec >= 1000000000) {
      ts.tv_nsec -= 1000000000;
      ts.tv_sec++;
    }
  }

  pthread_mutex_lock(&q->lock);

  while (1) {
    if (q->count > 0) {
      memcpy(data, q->dataPtr + q->idxRd * q->dataSize, q->dataSize);
      q->idxRd++;
      if (q->idxRd >= q->queueSize)
        q->idxRd = 0;
      if (q->count == q->queueSize)
        pthread_cond_signal(&q->condWr);
      q->count--;
      status = 0;
      break;
    } else {
      if (timeout_ms == 0)
        break;
      if (timeout_ms < 0)
        status = pthread_cond_wait(&q->condRd, &q->lock);
      else
        status = pthread_cond_timedwait(&q->condRd, &q->lock, &ts);

      if (status != 0) {
        status = -1;
        break;
      }
    }
  }

  pthread_mutex_unlock(&q->lock);

  return status;
}
/*end msg queue*/

/****************************************************************************
@Name         : cjPrintMemory
@Description  : 以16进制打印buffer内容
@Params in    : lineSize->每行显示的个数; addr->起始地址,可设置默认值0;
                pdata->buffer起始地址; len->要打印的长度
@Return       : 返回用户处理的返回值
*****************************************************************************/
bool sdPrintMemory(int lineSize, int addr, const void *pdata, int len) {
  int offset;
  int left;
  int tempLen;
  int i;

  if (lineSize == 0) {
    return FALSE;
  }

  printf("\ndispay data:\n");

  left = len;
  offset = addr;
  while (left) {
    if (offset / lineSize != (offset + left) / lineSize) {
      tempLen = lineSize - offset % lineSize;
    } else {
      tempLen = left;
    }

    printf("0x%.08x:", offset - offset % lineSize);

    for (i = 0; i < offset % lineSize; i++) {
      printf("      ");
    }
    for (i = 0; i < tempLen; i++) {
      printf(" 0x%.02x ", *((unsigned char *)pdata + len - left + i));
    }
    printf("\n");
    left -= tempLen;
    offset += tempLen;

  }
  printf("\n");

  return TRUE;
}

static void v_msgprint(const char *fmt, va_list ap) {
#define MAXLINE 128
  char buf[MAXLINE + 1];
  vsnprintf(buf, MAXLINE + 1, fmt, ap);
  write(2, buf, strlen(buf));
#undef MAXLINE
}

static void pd(int32_t level, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  v_msgprint(fmt, ap);
  va_end(ap);
}

void sdPrintMemoryEx(const char *name, const void *buf1, int32_t len) {
  int32_t level = 0;
  uint8_t *buf = (uint8_t *)buf1;
  //assert(buf != NULL);
  pd(level, "[%s]%s:\n", "DUMP", name);
  size_t i, j;
  const unsigned char *p;
  pd(level, "\
XX| 00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F | ........ ........ |\n");
  for (i = 0;; i++) {
    if (i % 16 == 0) {
      if (i < len)
        pd(level, "%2.2x| ", i / 16);
      else
        break;
    }
    if (i < len)
      pd(level, "%2.2x ", buf[i]);
    else
      pd(level, "   ");
    if (i % 16 == 7)
      pd(level, " ");
    if (i % 16 == 15) {
      pd(level, "| ");
      p = buf + i - 15;
      for (j = 0; j < 16; j++) {
        if (i - 15 + j < len) {
          if (isprint(p[j]))
            pd(level, "%c", p[j]);
          else
            pd(level, ".");
        } else {
          int k;

          if (j <= 7) {
            for (k = j; k < 17; k++)
              pd(level, " ");
          } else {
            for (k = j; k < 16; k++) {
              pd(level, " ");
            }
          }
          break;
        }

        if (j % 16 == 7)
          pd(level, " ");
      }

      pd(level, " |\n");
    }
  }

  fflush(stderr);
}

/*delete front space and back space*/
char *sdArgsTrim(char *s) {
  char *c = s + strlen(s) - 1;

  while (isspace(*c) && c > s) {
    *c = '\0';
    --c;
  }

  c = s;

  while (isspace(*c) && c < s + strlen(s) - 1) {
    c++;
  }

  return c;
}

typedef  struct _sd_mempool_s {
  HMUTEX       mutex;
  unsigned char   *addr;
  long        num;
  long        size;
  long        bitmap[4];
} sd_mempool_t;

HMEMPOOL      sdCreateMemPool(long num, long size) {
  sd_mempool_t *mempool;

  if (num > sizeof(long) * 8 * 4) {
    return (HMEMPOOL)NULL;
  }

  mempool = (sd_mempool_t *) malloc(sizeof(sd_mempool_t) + size * num);
  if (mempool == NULL) {
    return (HMEMPOOL)NULL;
  }

  memset(mempool, 0, sizeof(sd_mempool_t) + size * num);
  mempool->mutex = sdCreateMutex();
  mempool->num = num;
  mempool->size = size;
  mempool->addr = (unsigned char *)(mempool + 1);

  return (HMEMPOOL)mempool;
}

void        sdDestroyMemPool(HMEMPOOL mem) {
  sd_mempool_t *mempool = (sd_mempool_t *)mem;

  if (mempool == NULL) {
    return ;
  }

  sdDestroyMutex(mempool->mutex);
  free(mempool);

  return;
}

void         *sdMallocMemPool(HMEMPOOL mem) {
  int i;
  int k, q;
  sd_mempool_t *mempool = (sd_mempool_t *)mem;
  if (mempool == NULL) {
    return NULL;
  }

  sdMutexLock(mempool->mutex);

  for (i = 0; i < mempool->num; i++) {
    k = i % (sizeof(long) * 8);
    q = i / (sizeof(long) * 8);
    //printf("k = %d, q = %d\n", k, q);

    if (!(mempool->bitmap[q] & (1 << k))) {
      mempool->bitmap[q] |= 1 << k;
      sdMutexUnLock(mempool->mutex);
      return (void *)(mempool->addr + mempool->size * i);
    }
  }

  sdMutexUnLock(mempool->mutex);

  return NULL;
}

void        sdFreeMemPool(HMEMPOOL mem, void *addr) {
  int i;
  int k, q;
  sd_mempool_t *mempool = (sd_mempool_t *)mem;
  if (mempool == NULL) {
    return ;
  }

  sdMutexLock(mempool->mutex);
  for (i = 0; i < mempool->num; i++) {
    k = i % (sizeof(long) * 8);
    q = i / (sizeof(long) * 8);

    //printf("addr %p, k = %d q %d %p\n", addr, i, q ,(mempool->addr + mempool->size * i));
    if ((unsigned long)(mempool->addr + mempool->size * i) == (unsigned long)addr) {
      mempool->bitmap[q] &= ~(1 << k);
      break;
    }
  }
  sdMutexUnLock(mempool->mutex);

  return ;
}

typedef struct _sd_fifo_s {
  HMUTEX  hMutex;

  struct fifo *fifo;
} sd_fifo_t;

/* Note: args size must be pow of two. 2^n */
void *sdCreateFifo(uint32_t size) {
  sd_fifo_t *p = NULL;

  p = calloc(1, sizeof(sd_fifo_t));
  if (NULL == p) return NULL;

  p->fifo = fifo_alloc(size);
  if (NULL == p->fifo) return NULL;
  p->hMutex = sdCreateMutex();

  return p;
}

void sdDestroyFifo(void *handle) {
  sd_fifo_t *p = (sd_fifo_t *)handle;

  if (p != NULL) {
    sdDestroyMutex(p->hMutex);
    fifo_free(p->fifo);
    p->fifo = NULL;

    free(p);
  }
}

uint32_t sdPutFifo(void *handle, const void *buffer, uint32_t len) {
  sd_fifo_t *p = (sd_fifo_t *)handle;

  sdMutexLock(p->hMutex);
  len = fifo_put(p->fifo, buffer, len);
  sdMutexUnLock(p->hMutex);

  return len;
}

uint32_t sdGetFifo(void *handle, void *buffer, uint32_t len) {
  sd_fifo_t *p = (sd_fifo_t *)handle;

  sdMutexLock(p->hMutex);
  len = fifo_get(p->fifo, buffer, len);
  sdMutexUnLock(p->hMutex);

  return len;
}

uint32_t sdGetValidLenFifo(void *handle) {
  sd_fifo_t *p = (sd_fifo_t *)handle;
  uint32_t len;

  sdMutexLock(p->hMutex);
  len = fifo_len(p->fifo);
  sdMutexUnLock(p->hMutex);

  return len;
}

uint32_t sdGetRoomFifo(void *handle) {
  sd_fifo_t *p = (sd_fifo_t *)handle;
  uint32_t len;

  sdMutexLock(p->hMutex);
  len = fifo_room(p->fifo);
  sdMutexUnLock(p->hMutex);

  return len;
}

uint32_t sdSplitToken(const char *source, int32_t index, char token, char *dest) {
  int i;
  char *b, *e, tmp[1023];

  strcpy(tmp, source);
  b = tmp;
  for (i = 0; i < index; i++) {
    e = strchr(b, token);
    if (e) {
      e[0] = '\0';
      b = e + 1;
    } else {
      b = tmp;
      break;
    }
  }
  e = strchr(b, token);
  if (e) {
    e[0] = '\0';
  }

  if (i >= index && (b != NULL) && (*b != '\0')) {
    strcpy(dest, b);
    return RET_OK;
  }

  return RET_FAIL;
}

#define BC26_RESET_PIN (2)
void bc26_reset() {
  gpio_pad_select_gpio(BC26_RESET_PIN);
  gpio_set_direction(BC26_RESET_PIN, GPIO_MODE_OUTPUT);
  gpio_set_level(BC26_RESET_PIN, 1);
  sdSleepMs(200);
  gpio_set_level(BC26_RESET_PIN, 0);
  sdSleepMs(1000);
}

