#include <linux/module.h>
#include <linux/ini.h>


static int helloworld_init()
{
    printk("helloworld!\n")
    return 0;
}

static void helloworld_exit()
{
    printk("helloworld bye!\n")
    return 0;
}

module_init(helloworld_init);
module_exit(helloworld_exit)；

MODULE_LICENSE("GPL")
MODULE_AUTHOR("YF")
MODULE_VERSION("V1.0")
