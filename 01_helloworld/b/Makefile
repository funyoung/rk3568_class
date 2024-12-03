obj-m += b.o
#https://blog.51cto.com/u_11767891/5532636
KBUILD_EXTRA_SYMBOLS=/home/rk3588/rk3568itop/rk3568_class/01_helloworld/a/Module.symvers
KDIR=/home/rk3588/rk3568itop/rk3568_linux_5.10/kernel
PWD?=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
	echo $(PWD)
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order .*.cmd .*.mod
