为应用工程师提供API封装，1个头文件，两个库文件。

编译命令
xxx-gcc -c timer*.c dev_open.c

编译静态库
xxxx-ar rcs libtime.a timer*.o
xxxx-ar rcs libopen.a dev_open.o

编译应用app
xxxx-gcc -o app app.c -L./ -ltime -lopen
