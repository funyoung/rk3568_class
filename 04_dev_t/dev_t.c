#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

/** 设备号由主次设备号组成（12+20=32位)，有宏定义组合，分解.
  * 在linux虚拟文件系统/proc/devices里列举了系统当前所有设备号
  * #cat /proc/devices
  */

/* 定义主次设备号变量 */
static int major;
static int minor;

/* 通过驱动模块传参传递主次设备号 */
module_param(major, int, S_IRUGO);
MODULE_PARM_DESC(major,"e.g: major=1");

module_param(minor, int, S_IRUGO);
MODULE_PARM_DESC(minor,"e.g: minor=0");

/* 定义设备号dev_t类型变量 dev_num */
static dev_t dev_num;


static int __init dev_t_init(void)
{
    int ret;

    if (major) {
        dev_num = MKDEV(major, minor);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
        ret = register_chrdev_region(dev_num, 1, "chrdev_name");
        if (ret < 0) {
            printk("register_chrdev_region is error: %d\n", ret);
        }
        printk("register_chrdev_region is ok %d\n", ret);
    } else {
        /* 通过动态方式进行设备号注册 */
        ret = alloc_chrdev_region(&dev_num, 0, 1, "chrdev_name");
        if (ret < 0) {
            printk("alloc_chrdev_region is error: %d\n", ret);
        }
        printk("alloc_chrdev_region is ok %d\n", ret);

        /* 通过宏获取主次设备号 */
        major = MAJOR(dev_num);
        minor = MINOR(dev_num);
        printk("major is %d\n", major);
        printk("minor is %d\n", minor);
    }

    return 0;
}

static void __exit dev_t_exit(void)
{
    /* 释放字符驱动设备号 */
    unregister_chrdev_region(dev_num, 1); 
    printk("device num module exit\n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
