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
    unsigned int off;
    char read_buf[13] = {0};
    char read_buf1[19] = {0};

    file_name = argc > 1 ? argv[1] : "/dev/test";

    fd = open(file_name, O_RDWR, 666);
    if (fd < 0) {
        perror("app: open error\n");
        return fd;
    }

    write(fd, "hello world", 13);
    off = lseek(fd, 0, SEEK_CUR);  /* 读当前位置 */
    printf("off is %d\n", off);

    off = lseek(fd, 0, SEEK_SET); /* 偏移量置0 */
    printf("off is %d\n", off);

    read(fd, read_buf, sizeof(read_buf));
    printf("app read is %s\n", read_buf);

    off = lseek(fd, 0, SEEK_CUR);  /* 读当前位置 */
    printf("off is %d\n", off);

    off = lseek(fd, -1, SEEK_CUR);  /* 当前位置往前挪一位 */
    printf("off is %d\n", off);

    write(fd, "Linux", 6);
    off = lseek(fd, 0, SEEK_CUR);  /* 读当前位置 */
    printf("off is %d\n", off);

    off = lseek(fd, 0, SEEK_SET); /* 偏移量置0 */
    printf("off is %d\n", off);

    read(fd, read_buf1, sizeof(read_buf1));
    printf("app read is %s\n", read_buf1);

    off = lseek(fd, 0, SEEK_CUR);  /* 读当前位置 */
    printf("off is %d\n", off);

    close(fd);

    return 0;
}
