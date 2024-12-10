#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

/** 设备号由主次设备号组成（12+20=32位)，有宏定义组合，分解.
  * 在linux虚拟文件系统/proc/devices里列举了系统当前所有设备号
  * #cat /proc/devices
  */

/* 定义设备号dev_t类型变量 dev_num */
static dev_t dev_num;

/* 定义cdev设备结构体和file_operations操作结构体 */
static struct cdev cdev_test;
static struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,  /* 指向本模块，避免模块的操作正使用时被卸载 */
};


static int __init dev_t_init(void)
{
    int ret;
    int major, minor;

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

    /* 初始化设备结构体，并连接到设备操作结构体, 设置owner后添加字符设备 */
    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_test.owner = THIS_MODULE;
    ret = cdev_add(&cdev_test, dev_num, 1);
    if (ret < 0) {
        printk("cdev_add is error: %d\n", ret);
    }
    printk("cdev_add is ok\n");

    return 0;
}

static void __exit dev_t_exit(void)
{
    /* 删除字符设备, 在释放字符驱动设备号之前. */
    cdev_del(&cdev_test);

    /* 释放字符驱动设备号 */
    unregister_chrdev_region(dev_num, 1); 
    printk("device num module exit\n");
}

module_init(dev_t_init);
module_exit(dev_t_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
