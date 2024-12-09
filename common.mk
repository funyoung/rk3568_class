#KDIR=/home/rk3588/rk3568itop/rk3568_linux_5.10/kernel
KDIR=/home/topeet/work/rk356x_linux/kernel

all:
	make -C $(KDIR) M=$(PWD) modules
	echo $(PWD)

clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order .*.cmd .*.mod
