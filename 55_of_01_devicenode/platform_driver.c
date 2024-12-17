#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>

/* 定义变量和匹配规则 */
struct device_node *mydevice_node;
const struct of_device_id *mynode_match;
static struct of_device_id mynode_of_match[] = {
    {.compatible="my devicetree"},
    {},
};

/* 平台设备探索函数 */
static int my_platform_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "my_platform_probe: Probing platform device\n");

    /* 通过节点名称找设备节点 */
    mydevice_node = of_find_node_by_name(NULL, "myLed");
    printk("mydevice node is %s found by name\n", mydevice_node->name);

    /* 通过路径设备节点 */
    mydevice_node = of_find_node_by_path("/topeet/myLed");
    printk("mydevice node is %s found by path\n", mydevice_node->name);

    /* 获取父节点*/
    mydevice_node = of_get_parent(mydevice_node);
    printk("mydevice node's parent is %s\n", mydevice_node->name);

    /* 获取子节点*/
    mydevice_node = of_get_next_child(mydevice_node, NULL);
    printk("mylded's sibling node is %s\n", mydevice_node->name);

    /* 使用compatible值查找节点 */
    mydevice_node = of_find_compatible_node(NULL, NULL, "my devicetree");
    printk("mydevice node is %s found by compatible\n", mydevice_node->name);

    /* 根据of_device_id匹配表查找匹配节点 */
    mydevice_node = of_find_matching_node_and_match(NULL, mynode_of_match, &mynode_match);
    printk("mydevice node matched is %s\n", mydevice_node->name);

    return 0;
}

/* 平台设备的移除函数 */
static int my_platform_remove(struct platform_device *pdev)
{
    printk(KERN_INFO "my_platform_remove: Removing platform device\r");

    /* 清理设备特定的操作 */

    return 0;
}

const struct of_device_id of_match_table_id[] = {
    { .compatible = "my devicetree" },
};

/* 定义平台驱动结构体 */
static struct platform_driver my_platform_driver = {
    .probe = my_platform_probe,
    .remove = my_platform_remove,
    .driver = {
        .name = "my_platform_device",
        .owner = THIS_MODULE,
        .of_match_table = of_match_table_id,
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
