#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

int main(int argc, char *argv[])
{
    char *file_name;
    int fd;
    char buf[32] = {0};
    struct pollfd fds[1];

    int ret;

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    fds[0].fd = fd;
    fds[0].events = POLLIN;

    printf("app: read before\n");
    while (1) {
        /* 轮训文件是否可操作，超时3000ms */
        ret = poll(fds, 1, 3000);
        if (!ret) {
            printf("time out !!\n");
        } else if (fds[0].revents == POLLIN) {
            /* 如果返回事件是有数据可读取  */
            read(fd, buf, sizeof(buf));
            printf("app: buf is %s\n", buf);
            sleep(1);
        }
    }
    printf("app: read after\n");

    close(fd);

    return 0;
}
