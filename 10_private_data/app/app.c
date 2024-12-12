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
    /*char read_buf[32] = {0};*/
    char write_buf[32] = "nihao, private data";

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR, 0666);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    /*read(fd, read_buf, sizeof(read_buf));
    printf("app: read_buf is %s\r\n", read_buf);*/

    write(fd, write_buf, sizeof(write_buf));
    /*printf("app: writen\n");*/

    close(fd);

    return 0;
}
