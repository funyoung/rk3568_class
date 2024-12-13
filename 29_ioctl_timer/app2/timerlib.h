#ifndef __TIMELIB_H__
#define __TIMELIB_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

/* 调用ioctl宏定义命令 */
#define TIMER_OPEN _IO('L', 0)
#define TIMER_CLOSE _IO('L', 1)
#define TIMER_SET _IOW('L', 2, int)

int dev_open();
int timer_set(int fd, int arg);
int timer_open(int fd);
int timer_close(int fd);

#endif
