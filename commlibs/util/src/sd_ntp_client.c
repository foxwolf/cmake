#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <netdb.h>     /* gethostbyname */
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "sd_os.h"
#include "sd_util.h"
#include "sd_typedef.h"
#include "sd_ulog.h"

#ifdef _PRECISION_SIOCGSTAMP
  #include <sys/ioctl.h>
#endif

//#define ENABLE_DEBUG

/* XXXX fixme - non-automatic build configuration */
#ifdef linux
#include <sys/utsname.h>
#include <sys/time.h>
typedef u_int32_t __u32;
#include <sys/timex.h>
#else
//extern struct hostent *gethostbyname(const int *name);
extern int h_errno;
#define herror(hostname) \
    fprintf(stderr,"Error %d looking up hostname %s\n", h_errno,hostname)
typedef uint32_t __u32;
#endif

#define JAN_1970        0x83aa7e80      /* 2208988800 1970 - 1900 in seconds */
#define ZONE_DIFF8_HOURS (8*3600)
#define NTP_PORT (123)


/* How to multiply by 4294.967296 quickly (and not quite exactly)
 * without using floating point or greater than 32-bit integers.
 * If you want to fix the last 12 microseconds of error, add in
 * (2911*(x))>>28)
 */
#define NTPFRAC(x) ( 4294*(x) + ( (1981*(x))>>11 ) )

/* The reverse of the above, needed if we want to set our microsecond
 * clock (via settimeofday) based on the incoming time in NTP format.
 * Basically exact.
 */
#define USEC(x) ( ( (x) >> 12 ) - 759 * ( ( ( (x) >> 10 ) + 32768 ) >> 16 ) )

/* Converts NTP delay and dispersion, apparently in seconds scaled
 * by 65536, to microseconds.  RFC1305 states this time is in seconds,
 * doesn't mention the scaling.
 * Should somehow be the same as 1000000 * x / 65536
 */
#define sec2u(x) ( (x) * 15.2587890625 )

struct ntptime {
  unsigned int coarse;
  unsigned int fine;
};
typedef enum {
  NTP_PROCESS_OK      = 0,
  NTP_PROCESS_ERROR   = -1
} NTP_PROCESS_RESULT;

static int ntp_return = NTP_PROCESS_ERROR;
//extern int *optarg;



/* prototype for function defined in phaselock.c */
int contemplate_data(unsigned int absolute, double skew, double errorbar, int freq);

/* prototypes for some local routines */
void send_packet(int usd);
int rfc1305print(uint32_t *data, struct ntptime *arrival);
void udp_handle(int usd, int *data, int data_len, struct sockaddr *sa_source, int sa_len);

/* variables with file scope
 * (I know, bad form, but this is a short int program) */
static uint32_t incoming_word[325];
#define incoming ((int *) incoming_word)
#define sizeof_incoming (sizeof(incoming_word)*sizeof(uint32_t))
static struct timeval time_of_send;
static int live = 0;
static int set_clock = 0; /* non-zero presumably needs root privs */

/* when present, debug is a true global, shared with phaselock.c */
#ifdef ENABLE_DEBUG
  static int debug = 1;
  #define DEBUG_OPTION "d"
#else
  static int debug = 0;
  #define DEBUG_OPTION
#endif

int get_current_freq(void) {
  /* OS dependent routine to get the current value of clock frequency.
   */
#ifdef linux
  struct timex txc;
  txc.modes = 0;
  if (adjtimex(&txc) < 0) {
    perror("adjtimex");
    return (1);
  }
  return txc.freq;
#else
  return 0;
#endif
}

int set_freq(int new_freq) {
  /* OS dependent routine to set a new value of clock frequency.
   */
#ifdef linux
  struct timex txc;
  txc.modes = ADJ_FREQUENCY;
  txc.freq = new_freq;
  if (adjtimex(&txc) < 0) {
    perror("adjtimex");
    return (1);
  }
  return txc.freq;
#else
  return 0;
#endif
}

void send_packet(int usd) {
  __u32 data[12];
  struct timeval now;
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 4
#define PREC -6

  if (debug) { fprintf(stderr, "Sending ...\n"); }
  if (sizeof(data) != 48) {
    fprintf(stderr, "size error\n");
    return;
  }
  bzero((int *) data, sizeof(data));
  data[0] = htonl(
              (LI << 30) | (VN << 27) | (MODE << 24) |
              (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
  data[1] = htonl(1 << 16); /* Root Delay (seconds) */
  data[2] = htonl(1 << 16); /* Root Dispersion (seconds) */
  gettimeofday(&now, NULL);
  data[10] = htonl(now.tv_sec + JAN_1970); /* Transmit Timestamp coarse */
  data[11] = htonl(NTPFRAC(now.tv_usec));  /* Transmit Timestamp fine   */
  send(usd, data, 48, 0);
  time_of_send = now;
}

void get_packet_timestamp(int usd, struct ntptime *udp_arrival_ntp) {
  struct timeval udp_arrival;
#ifdef _PRECISION_SIOCGSTAMP
  if (ioctl(usd, SIOCGSTAMP, &udp_arrival) < 0) {
    perror("ioctl-SIOCGSTAMP");
    gettimeofday(&udp_arrival, NULL);
  }
#else
  gettimeofday(&udp_arrival, NULL);
#endif
  udp_arrival_ntp->coarse = udp_arrival.tv_sec + JAN_1970;
  udp_arrival_ntp->fine   = NTPFRAC(udp_arrival.tv_usec);
}

void check_source(int data_len, struct sockaddr *sa_source, int sa_len) {
  /* This is where one could check that the source is the server we expect */
  if (debug) {
    struct sockaddr_in *sa_in = (struct sockaddr_in *)sa_source;
    uWarn("packet of length %d received", data_len);
    if (sa_source->sa_family == AF_INET) {
      uWarn("Source: INET Port %d host %s",
            ntohs(sa_in->sin_port), inet_ntoa(sa_in->sin_addr));
    } else {
      uWarn("Source: Address family %d", sa_source->sa_family);
    }
  }
}

double ntpdiff(struct ntptime *start, struct ntptime *stop) {
  int a;
  unsigned int b;
  a = stop->coarse - start->coarse;
  if (stop->fine >= start->fine) {
    b = stop->fine - start->fine;
  } else {
    b = start->fine - stop->fine;
    b = ~b;
    a -= 1;
  }

  return a * 1.e6 + b * (1.e6 / 4294967296.0);
}

/* Does more than print, so this name is bogus.
 * It also makes time adjustments, both sudden (-s)
 * and phase-locking (-l).  */
/* return value is number of microseconds uncertainty in answer */
int rfc1305print(uint32_t *data, struct ntptime *arrival) {
  /* straight out of RFC-1305 Appendix A */
  int li, vn, mode, stratum, poll, prec;
  int delay, disp, refid;
  struct ntptime reftime, orgtime, rectime, xmttime;
  double el_time, st_time, skew1, skew2;
  int freq;

#define Data(i) ntohl(((uint32_t *)data)[i])
  li      = Data(0) >> 30 & 0x03;
  vn      = Data(0) >> 27 & 0x07;
  mode    = Data(0) >> 24 & 0x07;
  stratum = Data(0) >> 16 & 0xff;
  poll    = Data(0) >>  8 & 0xff;
  prec    = Data(0)       & 0xff;
  if (prec & 0x80) { prec |= 0xffffff00; }
  delay   = Data(1);
  disp    = Data(2);
  refid   = Data(3);
  reftime.coarse = Data(4);
  reftime.fine   = Data(5);
  orgtime.coarse = Data(6);
  orgtime.fine   = Data(7);
  rectime.coarse = Data(8);
  rectime.fine   = Data(9);
  xmttime.coarse = Data(10);
  xmttime.fine   = Data(11);
#undef Data

  if (set_clock) {   /* you'd better be root, or ntpclient will crash! */
    struct timeval tv_set;
    /* it would be even better to subtract half the slop */
    tv_set.tv_sec  = xmttime.coarse - JAN_1970;
    /* divide xmttime.fine by 4294.967296 */
    tv_set.tv_usec = USEC(xmttime.fine);
    tv_set.tv_sec  = tv_set.tv_sec + ZONE_DIFF8_HOURS;
    if (settimeofday(&tv_set, NULL) < 0) {
      perror("settimeofday");
      return (1);
    }
    ntp_return = NTP_PROCESS_OK;

    if (debug) {
      uWarn("set time to %lu.%.6lu", tv_set.tv_sec, tv_set.tv_usec);
    }
  }

  if (debug) {
    uWarn("LI=%d  VN=%d  Mode=%d  Stratum=%d  Poll=%d  Precision=%d",
          li, vn, mode, stratum, poll, prec);
    uWarn("Delay=%.1f  Dispersion=%.1f  Refid=%u.%u.%u.%u",
          sec2u(delay), sec2u(disp),
          refid >> 24 & 0xff, refid >> 16 & 0xff, refid >> 8 & 0xff, refid & 0xff);
    uWarn("Reference %u.%.10u", reftime.coarse, reftime.fine);
    uWarn("Originate %u.%.10u", orgtime.coarse, orgtime.fine);
    uWarn("Receive   %u.%.10u", rectime.coarse, rectime.fine);
    uWarn("Transmit  %u.%.10u", xmttime.coarse, xmttime.fine);
    uWarn("Our recv  %u.%.10u", arrival->coarse, arrival->fine);
  }
  el_time = ntpdiff(&orgtime, arrival); /* elapsed */
  st_time = ntpdiff(&rectime, &xmttime); /* stall */
  skew1 = ntpdiff(&orgtime, &rectime);
  skew2 = ntpdiff(&xmttime, arrival);
  freq = get_current_freq();
  if (debug) {
    uWarn("Total elapsed: %9.2f\n"
          "Server stall:  %9.2f\n"
          "Slop:          %9.2f\n",
          el_time, st_time, el_time - st_time);
    uWarn("Skew:          %9.2f\n"
          "Frequency:     %9d\n"
          " day   second     elapsed    stall     skew  dispersion  freq\n",
          (skew1 - skew2) / 2, freq);
  }
  /* Not the ideal order for printing, but we want to be sure
   * to do all the time-sensitive thinking (and time setting)
   * before we start the output, especially fflush() (which
   * could be slow).  Of course, if debug is turned on, speed
   * has gone down the drain anyway. */
  if (live) {
    int new_freq;
    new_freq = contemplate_data(arrival->coarse, (skew1 - skew2) / 2,
                                el_time + sec2u(disp), freq);
    if (!debug && new_freq != freq) { set_freq(new_freq); }
  }
  uWarn("%d %.5d.%.3d  %8.1f %8.1f  %8.1f %8.1f %9d",
        arrival->coarse / 86400, arrival->coarse % 86400,
        arrival->fine / 4294967, el_time, st_time,
        (skew1 - skew2) / 2, sec2u(disp), freq);
  fflush(stdout);
  return (el_time - st_time);
}

int stuff_net_addr(struct in_addr *p, const char *hostname) {
  struct hostent *ntpserver;
  ntpserver = gethostbyname(hostname);
  if (ntpserver == NULL) {
    herror(hostname);
    return (1);
  }
  if (ntpserver->h_length != 4) {
    fprintf(stderr, "oops %d\n", ntpserver->h_length);
    return (1);
  }
  memcpy(&(p->s_addr), ntpserver->h_addr_list[0], 4);
  return 0;
}

int setup_receive(int usd, unsigned int interface, short int port) {
  struct sockaddr_in sa_rcvr;
  bzero((int *) &sa_rcvr, sizeof(sa_rcvr));
  sa_rcvr.sin_family = AF_INET;
  sa_rcvr.sin_addr.s_addr = htonl(interface);
  sa_rcvr.sin_port = htons(port);
  if (bind(usd, (struct sockaddr *) &sa_rcvr, sizeof(sa_rcvr)) == -1) {
    fprintf(stderr, "could not bind to udp port %d\n", port);
    perror("bind");
    return (1);
  }
  listen(usd, 3);
  return 0;
}

int setup_transmit(int usd, const char *host, short int port) {
  struct sockaddr_in sa_dest;
  bzero((int *) &sa_dest, sizeof(sa_dest));
  sa_dest.sin_family = AF_INET;
  stuff_net_addr(&(sa_dest.sin_addr), host);
  sa_dest.sin_port = htons(port);
  fprintf(stderr, "  (%s|%s|%d)  \n", __FILE__, __func__, __LINE__);
  if (connect(usd, (struct sockaddr *)&sa_dest, sizeof(sa_dest)) == -1) {
    perror("connect");
    return (1);
  }
  return 0;
}

void primary_loop(int usd, int num_probes, int interval, int goodness) {
  fd_set fds;
  struct sockaddr sa_xmit;
  int i, pack_len, sa_xmit_len, probes_sent, error;
  struct timeval to;
  struct ntptime udp_arrival_ntp;

  if (debug) { uWarn("Listening..."); }

  probes_sent = 0;
  sa_xmit_len = sizeof(sa_xmit);
  to.tv_sec = 0;
  to.tv_usec = 0;
  for (;;) {
    fprintf(stderr, "  (%s|%s|%d)  \n", __FILE__, __func__, __LINE__);
    FD_ZERO(&fds);
    FD_SET(usd, &fds);
    i = select(usd + 1, &fds, NULL, NULL, &to); /* Wait on read or error */
    if ((i != 1) || (!FD_ISSET(usd, &fds))) {
      if (i == EINTR) { continue; }
      if (i < 0) { perror("select"); }
      if (to.tv_sec == 0) {
        if (probes_sent >= num_probes &&
            num_probes != 0) {
          fprintf(stderr, "  (%s|%s|%d) \n", __FILE__, __func__, __LINE__);
          break;
        }
        send_packet(usd);
        ++probes_sent;
        to.tv_sec = interval;
        to.tv_usec = 0;
      }
      continue;
    }
    fprintf(stderr, "  (%s|%s|%d)  \n", __FILE__, __func__, __LINE__);
    pack_len = recvfrom(usd, incoming, sizeof_incoming, 0,
                        &sa_xmit, (socklen_t *)&sa_xmit_len);
    fprintf(stderr, "  (%s|%s|%d)  \n", __FILE__, __func__, __LINE__);
    error = goodness + 1;
    if (pack_len < 0) {
      perror("recvfrom");
    } else if (pack_len > 0 && (unsigned)pack_len < sizeof_incoming) {
      get_packet_timestamp(usd, &udp_arrival_ntp);
      check_source(pack_len, &sa_xmit, sa_xmit_len);
      error = rfc1305print(incoming_word, &udp_arrival_ntp);
      /* udp_handle(usd,incoming,pack_len,&sa_xmit,sa_xmit_len); */
    } else {
      uWarn("Ooops.  pack_len=%d", pack_len);
      fflush(stdout);
    }
    if (error < goodness && goodness != 0) { break; }
    if (probes_sent >= num_probes && num_probes != 0) { break; }
  }
}

void usage(int *argv0) {
  fprintf(stderr,
          "Usage: %n [-c count] [-d] [-g goodness] -h hostname [-i interval]\n"
          "\t[-l] [-p port] [-r] [-s] \n",
          argv0);
}

int ntp_get_time(const char *ntp_host_name) {
  int usd;  /* socket */
  //int c;
  /* These parameters are settable from the command line
     the initializations here provide default behavior */
  short int  udp_local_port    = 0;    /* default of 0 means kernel chooses */
  int cycle_time              = 600;  /* seconds */
  int probe_count             = 0;    /* default of 0 means loop forever */
  //int debug                   = 1;
  int goodness                = 0;
  const char *hostname              = NULL; /* must be set */
  //int replay                  = 0;    /* replay mode overrides everything */
  ntp_return = NTP_PROCESS_ERROR;
  if (ntp_host_name == NULL) {
    return ntp_return;
  }

  //*************************************************************
  set_clock++;
  probe_count = 1;
  cycle_time  = 5;
  hostname    = ntp_host_name;
  //*************************************************************

  if (debug) {
    uWarn("Configuration:\n"
          "  -c probe_count %d\n"
          "  -d (debug)     %d\n"
          "  -g goodness    %d\n"
          "  -h hostname    %s\n"
          "  -i interval    %d\n"
          "  -l live        %d\n"
          "  -p local_port  %d\n"
          "  -s set_clock   %d\n",
          probe_count, debug, goodness, hostname, cycle_time,
          live, udp_local_port, set_clock);
  }

  /* Startup sequence */
  if ((usd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket");
    return (ntp_return);
  }
  uWarn("ntp check time");

  setup_receive(usd, INADDR_ANY, udp_local_port);
  setup_transmit(usd, hostname, NTP_PORT);
  primary_loop(usd, probe_count, cycle_time, goodness);
  close(usd);
  if (debug) {
    uWarn("ntp result = %d", ntp_return);
  }
  return ntp_return;
}

/*
int main(int argc, int *argv[]){
    ntp_get_time(argv[1]);
}
*/


