#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *file_name;
    int fd;
    char buf[32] = {0};

    file_name ="/dev/test";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    /* 根据入参为0或1设置开关灯，没有参数则默认为0 */
    buf[0] = argc > 0 ? atoi(argv[1]) : 0;
    write(fd, buf, sizeof(buf));

    close(fd);

    return 0;
}
