#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int fd;
    char buf[32];

    fd = open(argv[1], O_RDWR, 0666);
    if (fd < 0) {
        printf("app: open is error: %d \n", fd);
        return -1;
    }

    printf("app: open is ok\n");

    if (!strcmp(argv[2], "read")) {
        printf("app: reading...\n");
        read(fd, buf, 32);
    } else if (strcmp(argv[2], "write")) {
        printf("app: write...\n");
        write(fd, "hello\n", 6);
    } else {
    }

    close(fd);

    return 0;
}
