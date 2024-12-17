#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>

/* 定义设备节点的变量 */
static struct device_node *mydevice_node;
static int num;
static u32 value_u32;
static u64 value_u64;
static u32 out_value[2];
static const char *value_compatible;
static struct property *my_property;

/* 平台设备探索函数 */
static int my_platform_probe(struct platform_device *pdev)
{
    printk(KERN_INFO "my_platform_probe: Probing platform device\n");

    /* 通过节点名称找设备节点 */
    mydevice_node = of_find_node_by_name(NULL, "myLed");
    printk("mydevice node is %s found by name\n", mydevice_node->name);

    /* 查找compatible属性 */
    my_property = of_find_property(mydevice_node, "compatible", NULL);
    printk("my_property name is %s\n", my_property->name);

    /* 读reg属性的元素数量 */
    num = of_property_count_elems_of_size(mydevice_node, "reg", 4);
    printk("reg num is %d\n", num);

    /* 读reg属性的值 */
    of_property_read_u32_index(mydevice_node, "reg", 0, &value_u32);
    of_property_read_u64_index(mydevice_node, "reg", 0, &value_u64);
    printk("value u32 is 0x%X\n", value_u32);
    printk("value u64 is 0x%llx\n", value_u64);

    /* 读reg属性变长数组 */
    of_property_read_variable_u32_array(mydevice_node, "reg", out_value, 1, 2);
    printk("out_value[0] is 0x%X\n", out_value[0]);
    printk("out_value[1] is 0x%X\n", out_value[1]);

    /* 读compatible属性字符串值*/
    of_property_read_string(mydevice_node, "compatible", &value_compatible);
    printk("compatible value is %s\n", value_compatible);

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
