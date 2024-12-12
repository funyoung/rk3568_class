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

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    printf("app: read before\n");
    read(fd, buf, sizeof(buf));
    printf("app: buf is %s\n", buf);
    printf("app: read after\n");

    close(fd);

    return 0;
}
