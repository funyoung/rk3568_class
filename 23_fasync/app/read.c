#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

static int fd;
static char buf[32] = {0};

/* SIGIO信号的信号处理函数 */
static void func(int signum)
{
    read(fd, buf, 32);
    printf("read app: buf is %s\n", buf);
}

int main(int argc, char *argv[])
{
    char *file_name;

    int ret;
    int flags;

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    /* 注册SIGIO信号处理函数 */
    signal(SIGIO, func);

    /* 设置接受信号的进程id */
    fcntl(fd, F_SETOWN, getpid());

    /* 设置文件描述符标志 */
    flags = fcntl(fd, F_GETFD);
    fcntl(fd, F_SETFL, flags | FASYNC);

    while (1);

    close(fd);

    return 0;
}
