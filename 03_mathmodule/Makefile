### build with '../mkmod3568.sh'

#https://blog.51cto.com/u_11767891/5532636
#KBUILD_EXTRA_SYMBOLS=/home/rk3588/rk3568itop/rk3568_class/01_helloworld/a/Module.symvers

obj-m := mathmodule.o
obj-m += hello.o

PWD?=$(shell pwd)

# set if exported symbol was in 'other' path
#KBUILD_EXTRA_SYMBOLS=$(PWD)/other/Module.symvers
KBUILD_EXTRA_SYMBOLS=$(PWD)/Module.symvers

include $(PWD)/../common.mk
