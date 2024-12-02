obj-m += helloworld.o
KDIR=/home/rk3588/rk3568itop/rk3568_linux_5.10/kernel
PWD?=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	echo $(PWD)
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order .*.cmd .*.mod
