#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *file_name;
    int fd;
    int count;

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    while (1) {
        read(fd, &count, sizeof(count));
        printf("app: num is %d\n", count);
        sleep(1);
    }

    close(fd);

    return 0;
}
