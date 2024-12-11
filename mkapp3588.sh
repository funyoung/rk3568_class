#/bin/sh

src="app.c"
if [ $# -gt 0 ]; then
#    echo "提供了多个参数。"
    src=$@
else
#    echo "参数个数不大于0。"
    src="app.c"
fi

/home/topeet/work/rk356x_linux/prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc -o app $src -static
