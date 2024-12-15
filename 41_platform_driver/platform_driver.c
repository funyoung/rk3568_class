#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

/* 平台设备探索函数 */
static int my_platform_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "my_platform_probe: Probing platform device\n");
    /* 添加设备特定的操作 */
    return 0;
}

/* 平台设备的移除函数 */
static int my_platform_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "my_platform_remove: Removing platform device\r");

    /* 清理设备特定的操作 */

    return 0;
}

/* 定义平台驱动结构体 */
static struct platform_driver my_platform_driver = {
    .probe = my_platform_probe,
    .remove = my_platform_remove,
    .driver = {
        .name = "my_platform_device",
        .owner = THIS_MODULE,
    }
};

static int __init my_platform_driver_init(void)
{
    int ret;

    /* 注册平台驱动 */
    ret = platform_driver_register(&my_platform_driver);

    if (ret) {
        printk(KERN_ERR "Failed to register platform driver\n");
    }

    printk(KERN_INFO "my_platform_driver: Platform driver initialized\n");
    return 0;
}

static void __exit my_platform_driver_exit(void)
{
    /* 注销平台驱动 */
    platform_driver_unregister(&my_platform_driver); 
    printk(KERN_INFO "my_platform_driver: Platform driver exited\n");
}

module_init(my_platform_driver_init);
module_exit(my_platform_driver_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
