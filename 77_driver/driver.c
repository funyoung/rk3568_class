#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/sysfs.h>

/* 导入自定义bus总线变量 */
extern struct bus_type mybus;

/* 定义驱动及其回调函数 */
static int mydriver_remove(struct device *dev)
{
    printk("this is %s\n", __func__);
    return 0;
}
static int mydriver_probe(struct device *dev)
{
    printk("this is %s\n", __func__);
    return 0;
}
static struct device_driver mydriver = {
    .name = "mydevice",  /* 同设备名匹配？ */
    .bus = &mybus,
    .probe = mydriver_probe,
    .remove = mydriver_remove,
};

static int __init mydriver_init(void)
{
    int ret;

    ret = driver_register(&mydriver);
    if (ret < 0) {
        printk(KERN_ERR "driver_register error.\n");
        return -EINVAL;
    }

    return ret;
}

static void __exit mydriver_exit(void)
{
    driver_unregister(&mydriver);
}


module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
