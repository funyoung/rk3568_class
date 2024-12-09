#/bin/sh

export ARCH=arm64
#export CROSS_COMPILE=/home/rk3588/rk3568itop/rk3568_linux_5.10/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-rockchip1031-linux-gnu-
export CROSS_COMPILE=/home/topeet/work/rk356x_linux/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
#echo "make $@"
make $@
