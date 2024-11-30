obj-m += helloworld.o
KDIR=/home/rk3588/rk3568itop/linux/...
PWD?=$(shell pwd)
all:
    make -C $(KDIR) M=$(PWD) modules
    echo $(PWD)
clean:
    rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order
