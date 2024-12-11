#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/** 设备号由主次设备号组成（12+20=32位)，有宏定义组合，分解.
  * 在linux虚拟文件系统/proc/devices里列举了系统当前所有设备号
  * #cat /proc/devices
  */

/* 定义设备号dev_t类型变量 dev_num */
static dev_t dev_num;


static int cdev_test_open(struct inode *inode, struct file *file)
{
    printk("This is cdev_test_open \n");
    return 0;
}

static ssize_t cdev_test_read (struct file *file, char __user *buf, size_t size, loff_t *off)
{
    /* 内核空间的字符串数据缓冲，复制到用户空间返回 */
    char kbuf[32] = "This is cdev_test_read!";
    if (copy_to_user(buf, kbuf, strlen(kbuf)) != 0) {
        printk("copy_to_user error\r\n");
        return -1;
    }

    printk("This is cdev_test_read \r\n");
    return 0;
}

static ssize_t cdev_test_write (struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    /* 用户空间传来的数据复制到内核缓冲区 */
    char kbuf[32] = {0};
    if (copy_from_user(kbuf, buf, size) != 0) {
        printk("copy_from_user error\r\n");
        return -1;
    }
    printk("This is cdev_test_write \n");
    printk("kbuf is %s\r\n", kbuf);
    return 0;
}

static int cdev_test_release (struct inode *inode, struct file *file)
{
    printk("This is cdev_test_realse \n");
    return 0;
}


/* 定义cdev设备结构体和file_operations操作结构体 */
static struct cdev cdev_test;
static struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,  /* 指向本模块，避免模块的操作正使用时被卸载 */
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release,
};

/* 要创建的类class指针变量 */
static struct class *class_test;


static int __init chr_fops_init(void)
{
    int ret;
    int major, minor;

    /* 通过动态方式进行设备号注册 */
    ret = alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");
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
        // todo: goto or return here?
    }
    printk("cdev_add is ok\n");

    /* 创建名为class_test的类，然后创建名称test的设备 */
    class_test = class_create(THIS_MODULE, "class_test");
    device_create(class_test, NULL, dev_num, NULL, "test"); /*设备的父节点和数据用NULL*/

    return 0;
}

static void __exit chr_fops_exit(void)
{
    /* 删除字符设备, 在释放字符驱动设备号之前. */
    cdev_del(&cdev_test);

    /* 释放字符驱动设备号 */
    unregister_chrdev_region(dev_num, 1); 

    /* 按顺序删除创建的设备，和类 */
    device_destroy(class_test, dev_num);
    class_destroy(class_test);
    printk("module exit\n");
}

module_init(chr_fops_init);
module_exit(chr_fops_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");