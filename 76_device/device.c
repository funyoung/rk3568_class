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

/* 定义设备变量hard code以及其释放函数 */
static void myrelease(struct device *dev)
{
    printk("this is %s\n", __func__);
}
static struct device mydevice = {
    .init_name = "mydevice",
    .bus = &mybus,
    .release = myrelease,
    .devt = ((255<<20|0)),
};

static int __init device_init(void)
{
    int ret;

    ret = device_register(&mydevice);
    if (ret < 0) {
        printk(KERN_ERR "device_register error.\n");
        return -EINVAL;
    }

    return 0;
}

static void __exit device_exit(void)
{
    device_unregister(&mydevice);
}


module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
