### build driver module with '../../mkmod3568.sh'
### set KBUILD_EXTRA_SYMBOLS if 'add' is undefined

obj-m += b.o
#https://blog.51cto.com/u_11767891/5532636
#KBUILD_EXTRA_SYMBOLS=/home/rk3588/rk3568itop/rk3568_class/01_helloworld/a/Module.symvers
PWD?=$(shell pwd)
#KBUILD_EXTRA_SYMBOLS=/home/topeet/work/rk356x_class/01_helloworld/a/Module.symvers
KBUILD_EXTRA_SYMBOLS=$(PWD)/../a/Module.symvers

include $(PWD)/../../common.mk
