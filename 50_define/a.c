#include <linux/module.h>
#include <linux/init.h>


static int helloworld_init(void)
{
#ifndef DEBUG
    printk("helloworld branch a!\n");
#else
    printk("helloworld branch b!\n");
    /* printk("DEBUG is %d\n", DEBUG); */
#endif
    return 0;
}

static void helloworld_exit(void)
{
    printk("helloworld bye!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
