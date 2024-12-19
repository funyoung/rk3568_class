#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/sysfs.h>

/* 自定义总线，含设备和驱动匹配的回调和匹配上后的探测回调 */
static int mybus_match(struct device *dev, struct device_driver *drv)
{
    return strcmp(dev_name(dev), drv->name) == 0;
}
static int mybus_probe(struct device *dev)
{
    struct device_driver *drv = dev->driver;
    if (drv->probe)
    {
        drv->probe(dev);
    } else {
        printk("%s: no probe function with the driver(%p) of device(%p)", __func__, drv, dev);
    }

    return 0;
}
static struct bus_type mybus = {
    .name = "mybus",
    .match = mybus_match,
    .probe = mybus_probe,
};

/* 定义总线的属性 */
static ssize_t mybus_show(struct bus_type *bus, char *buf)
{
    return sprintf(buf, "%s\n", __func__);
}
static struct bus_attribute mybus_attr = {
    .attr = {
        .name = "value",
        .mode = 0664,
    },
    .show = mybus_show,
};

static int __init bus_init(void)
{
    int ret;

    /* 注册总线，并在sysfs中创建属性文件 */
    ret = bus_register(&mybus);
    if (ret < 0) {
        printk(KERN_ERR "bus_register error\n");
    }

    ret = bus_create_file(&mybus, &mybus_attr);
    if (ret < 0) {
        printk(KERN_ERR "bus_create_file error\n");
    }

    return 0;
}

static void __exit bus_exit(void)
{
    /* 从sysfs移除属性文件 */
    bus_remove_file(&mybus, &mybus_attr);
    /* 注销总线 */
    bus_unregister(&mybus);
}


module_init(bus_init);
module_exit(bus_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
