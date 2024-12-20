#include <linux/module.h>
#include <linux/init.h>

/* copy Module.symvers with 'add' first, e.g., from project a/ */
extern int add(int a, int b);

static int __init helloworld_init(void)
{
    int a = 0;
    a = add(2, 3);
    printk("a = %d\n", a);
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
