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
#define CMD_TEST _IO('L', 0)
#define CMD_WRITE _IOW('L', 1, int)
#define CMD_READ _IOR('L', 2, int)

int main(int argc, char *argv[])
{
    char *file_name;
    char *option;
    int fd;
    int value;

    option = argc > 1 ? argv[1] : "read";
    file_name = argc > 2 ? argv[2] : "/dev/test";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    if (!strcmp(option, "write")) {
        ioctl(fd, CMD_WRITE, 1);
        printf("app: ioctl set 1\n");
    }


    if (!strcmp(option, "read")) {
        ioctl(fd, CMD_READ, &value);
        printf("app: ioctl get value %d\n", value);
    }

    close(fd);

    return 0;
}
