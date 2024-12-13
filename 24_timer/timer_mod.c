#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

/* 定义一个定时器，超时打印信息后，在循环定时. */
static void function_test(struct timer_list *t);
DEFINE_TIMER(timer_test, function_test);
static void function_test(struct timer_list *t)
{
    printk("this is timeout function test \n");
    /* 设置下次超时在5秒后 */
    mod_timer(&timer_test, jiffies_64 + msecs_to_jiffies(5000));
}

static int __init timer_mod_init(void)
{
    /* 5秒后首次超时 */
    timer_test.expires = jiffies_64 + msecs_to_jiffies(5000);
    add_timer(&timer_test);
    return 0;
}

static void __exit timer_mod_exit(void)
{
    /* 删除一个定时器 */
    del_timer(&timer_test);
    printk("module exit!\n");
}

module_init(timer_mod_init);
module_exit(timer_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
