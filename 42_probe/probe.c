#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

/* 平台设备探索函数 */
static int my_platform_probe(struct platform_device *pdev)
{
    struct resource *res_mem, *res_irq;

    printk(KERN_INFO "my_platform_probe: Probing platform device\n");
    /* 添加设备特定的操作 */
    /* 两种方法从pdev中读取硬件设备信息 */

    if (pdev->num_resources < 2) {
        printk(KERN_ERR "invalid resource count %d\n", pdev->num_resources);
        return -ENODEV;
    }

    res_mem = &pdev->resource[0];
    res_irq = &pdev->resource[1];
    printk("Method 1: mem resource start: %llx, end: %llx\n", res_mem->start, res_mem->end);
    printk("Method 1: irq resource number: %lld\n", res_irq->start);

    res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res_mem) {
        dev_err(&pdev->dev, "Failed to get memory resource\n");
        return -ENODEV;
    }

    res_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
    if (!res_irq) {
        dev_err(&pdev->dev, "Failed to get IRQ resource\n");
        return -ENODEV;
    }

    printk("Method 2: mem resource start: %llx, end: %llx\n", res_mem->start, res_mem->end);
    printk("Method 2: irq resource number: %lld\n", res_irq->start);

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
