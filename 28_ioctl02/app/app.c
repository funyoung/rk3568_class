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
#define CMD_WRITE _IOW('L', 0, int)

/* 定义参数结构体 */
struct args {
    int a, b, c;
};

int main(int argc, char *argv[])
{
    char *file_name;
    int fd;
    struct args test;

    test.a = 11;
    test.b = 22;
    test.c = 33;

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR, 0777);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    ioctl(fd, CMD_WRITE, &test);

    close(fd);

    return 0;
}
