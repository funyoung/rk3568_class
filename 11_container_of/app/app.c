#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *first_name, *second_name;
    int fd1, fd2;
    /*char read_buf[32] = {0};*/
    char first_buf[32] = "hi, first /dev/test1";
    char second_buf[32] = "hello,  second /dev/test2";

    /* 测试设备名，根据驱动创建的/dev节点 */
    first_name = argc > 1 ? argv[1] : "/dev/test1";
    second_name = argc > 2 ? argv[2] : "/dev/test2";

    fd1 = open(first_name, O_RDWR, 0666);
    if (fd1 < 0) {
        perror("app: open error\n");
        return fd1;
    }

    write(fd1, first_buf, sizeof(first_buf));
    close(fd1);

    fd2 = open(second_name, O_RDWR, 0666);
    if (fd2 < 0) {
        perror("app: open error \n");
        return fd2;
    }

    write(fd2, second_buf, sizeof(second_buf));
    close(fd2);

    return 0;
}
