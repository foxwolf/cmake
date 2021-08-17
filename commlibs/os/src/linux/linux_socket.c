/**
 * @file linux_socket.c
 * @brief socket
 *
 * @author  <support@bjsdwlkj.cn>
 * @version 1.0.0
 * @date 2021-02-25
 */
/* Copyright (C) 2018-2021, SDWL Inc.
 * All right reserved
 */

#include "sd_os.h"

int set_ipaddr(unsigned long iaddr, char *iface) {
  int skfd;
  struct ifreq ifr;
  struct sockaddr_in *psin;
  int err;

  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }

  strcpy(ifr.ifr_name, iface);
  psin = (struct sockaddr_in *)&ifr.ifr_addr;
  psin->sin_family = AF_INET;
  psin->sin_addr.s_addr = iaddr;
  err = ioctl(skfd, SIOCSIFADDR, &ifr);
  close(skfd);

  return (err);
}

int set_netmask(unsigned long mask, char *iface) {
  int skfd;
  struct ifreq ifr;
  struct sockaddr_in *psin;
  int err;

  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }

  strcpy(ifr.ifr_name, iface);
  psin = (struct sockaddr_in *)&ifr.ifr_addr;
  psin->sin_family = AF_INET;
  psin->sin_addr.s_addr = mask;
  err = ioctl(skfd, SIOCSIFNETMASK, &ifr);
  close(skfd);

  return err;
}

int set_defgw(unsigned long gw, char *iface) {
  int skfd;
  struct rtentry rt;
  int err;

  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }

  /* Delete existing defalt gateway */

  memset(&rt, 0, sizeof(rt));

  rt.rt_dev = iface;
  rt.rt_dst.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;

  rt.rt_genmask.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;

  rt.rt_flags = RTF_UP;

  err = ioctl(skfd, SIOCDELRT, &rt);


  if ((err == 0 || errno == ESRCH) && gw) {
    /* Set default gateway */
    memset(&rt, 0, sizeof(rt));

    rt.rt_dev = iface;
    rt.rt_dst.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = 0;

    rt.rt_gateway.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_gateway)->sin_addr.s_addr = gw;

    rt.rt_genmask.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = 0;

    rt.rt_flags = RTF_UP | RTF_GATEWAY;

    err = ioctl(skfd, SIOCADDRT, &rt);
  }

  close(skfd);

  return err;
}

int get_ipaddr(unsigned long *p, char *iface) {
  int skfd;
  struct ifreq ifr;
  int ret = -1;

  *p = INADDR_NONE;
  strcpy(ifr.ifr_name, iface);
  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }
  if (ioctl(skfd, SIOCGIFADDR, &ifr) >= 0) {
    *p = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
    ret = 0;
  }
  close(skfd);

  return ret;
}

int get_netmask(unsigned long *p, char *iface) {
  int skfd;
  struct ifreq ifr;
  int ret = -1;

  strcpy(ifr.ifr_name, iface);
  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }
  if (ioctl(skfd, SIOCGIFNETMASK, &ifr) >= 0) {
    *p = ((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr.s_addr;
    ret = 0;
  }
  close(skfd);

  return ret;
}

int get_gateway(unsigned long *p, char *netname) {
  FILE *fp;
  char buf[256];  // 128 is enough for linux
  char iface[16];
  unsigned long dest_addr, gate_addr;

  *p = INADDR_NONE;
  fp = fopen("/proc/net/route", "r");
  if (fp == NULL) {
    return -1;
  }

  /* Skip title line */
  fgets(buf, sizeof(buf), fp);

  while (fgets(buf, sizeof(buf), fp)) {
    if (sscanf(buf, "%s\t%lX\t%lX", iface, &dest_addr, &gate_addr) != 3 || dest_addr != 0)
      continue;
    if (!strcmp(netname, iface)) {
      *p = gate_addr;
      fclose(fp);
      return 0;
    }
  }

  fclose(fp);
  return -1;
}

/* Get ethernet/MAC address */
int get_macaddr(char *cp, char *iface) {
  int skfd;
  struct ifreq ifr;
  unsigned char *p;
  int err;

  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }

  strcpy(ifr.ifr_name, iface);
  err = ioctl(skfd, SIOCGIFHWADDR, &ifr);
  if (err == 0) {
    p = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    sprintf(cp, "%02X:%02X:%02X:%02X:%02X:%02X", p[0], p[1], p[2], p[3], p[4], p[5]);
  }

  close(skfd);

  return err;
}

int del_rt(unsigned long dest, unsigned long gw, unsigned long mask, char *iface) {
  int skfd;
  struct rtentry rt;
  int err;

  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }
  /* Delete existing defalt gateway */
  memset(&rt, 0, sizeof(rt));

  rt.rt_dst.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = dest;

  rt.rt_genmask.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = mask;

  rt.rt_gateway.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_gateway)->sin_addr.s_addr = gw;

  rt.rt_dev = iface;
  if (gw) {
    rt.rt_flags |= RTF_GATEWAY;
  }

  rt.rt_flags = RTF_UP;
  err = ioctl(skfd, SIOCDELRT, &rt);

  close(skfd);

  return err;
}

int add_rt(unsigned long dest, unsigned long gw, unsigned long mask, char *iface) {
  int skfd;
  struct rtentry rt;
  int err;

  skfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (skfd < 0) {
    return -1;
  }
  /* Delete existing defalt gateway */
  memset(&rt, 0, sizeof(rt));

  rt.rt_dst.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = dest;

  rt.rt_genmask.sa_family = AF_INET;
  ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = mask;

  rt.rt_flags = RTF_UP;
  err = ioctl(skfd, SIOCDELRT, &rt);

  if (err == 0 || errno == ESRCH) {
    memset(&rt, 0, sizeof(rt));

    rt.rt_dst.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_dst)->sin_addr.s_addr = dest;

    rt.rt_gateway.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_gateway)->sin_addr.s_addr = gw;

    rt.rt_genmask.sa_family = AF_INET;
    ((struct sockaddr_in *)&rt.rt_genmask)->sin_addr.s_addr = mask;

    rt.rt_flags = RTF_UP;// | RTF_HOST;
    if (gw) {
      rt.rt_flags |= RTF_GATEWAY;
    }

    rt.rt_dev = iface;

    err = ioctl(skfd, SIOCADDRT, &rt);
  }

  close(skfd);

  return err;
}


