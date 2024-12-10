#include <linux/module.h>
#include <linux/init.h>

/* export symbol 'add' */
extern int add(int a, int b);

int add(int a, int b)
{
    return a + b;
}
EXPORT_SYMBOL(add);

static int __init helloworld_init(void)
{
    printk("helloworld!\n");
    return 0;
}

static void __exit helloworld_exit(void)
{
    printk("helloworld bye!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
