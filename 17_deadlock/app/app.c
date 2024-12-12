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
    char *option;
    int fd;

    file_name = argc > 1 ? argv[1] : "/dev/test";
    option = argc > 2 ? argv[2] : "itop";

    fd = open(file_name, O_RDWR);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    if (strcmp(option, "topeet") == 0) {
        write(fd, "topeet", 10);
    } else if (strcmp(option, "itop") == 0) {
        write(fd, "itop", 10);
    }

    close(fd);

    return 0;
}
