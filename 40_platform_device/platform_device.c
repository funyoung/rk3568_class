#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#define MEM_START_ADDR 0xFDD60000
#define MEM_END_ADDR 0xFDD60004
#define IRQ_NUMBER 101

static struct resource my_resources[] = {
    {
        .start = MEM_START_ADDR,
        .end = MEM_END_ADDR,
        .flags = IORESOURCE_MEM,
    },
    {
        .start = IRQ_NUMBER,
        .end = IRQ_NUMBER,
        .flags = IORESOURCE_IRQ,
    },
};

static void my_platform_device_relese(struct device *dev)
{
    // 释放资源的回到函数
    printk(KERN_INFO "this is my_platform_device_relese\n");
}

static struct platform_device my_platform_device = {
    .name = "my_platform_device",
    .id = -1,
    .num_resources = ARRAY_SIZE(my_resources),
    .resource = my_resources,
    .dev.release = my_platform_device_relese,
};

static int __init platform_device_init(void)
{
    int ret;

    /* 注册平台设备 */
    ret = platform_device_register(&my_platform_device);

    if (ret) {
        printk(KERN_ERR "Failed to register platform device\n");
    }

    printk(KERN_INFO "platform device registered\n");
    return 0;
}

static void __exit platform_device_exit(void)
{
    /* 注销平台设备 */
    platform_device_unregister(&my_platform_device); 
    printk(KERN_INFO "platform device unregistered\n");
}

module_init(platform_device_init);
module_exit(platform_device_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
