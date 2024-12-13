#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/atomic.h>

/** 设备号由主次设备号组成（12+20=32位)，有宏定义组合，分解.
  * 在linux虚拟文件系统/proc/devices里列举了系统当前所有设备号
  * #cat /proc/devices
  */

/* 定义文件私有数据的结构, 信息封装设备号，字符设备，类和设备，缓冲区等 */
struct chrdev_test {
    /* 定义设备号dev_t类型变量 dev_num */
    dev_t dev_num;
    int major;
    int minor;

    /* 定义cdev设备结构体和file_operations操作结构体 */
    struct cdev cdev_test;

    /* 要创建的类class指针变量 */
    struct class *class_test;

    /* device_create 返回，暂时用不到 */
    struct device *device;

    /* second count */
    int sec;
};

/* 定义文件私有数据结构体的变量，open时赋值，透传给read,write方法 */
static struct chrdev_test dev1;

/* 定义原子操作，同一时间只有一个应用访问驱动 */
static atomic64_t v = ATOMIC_INIT(0);

static void function_test(struct timer_list *t);
DEFINE_TIMER(timer_test, function_test);
static void function_test(struct timer_list *t)
{
    /* 原子操作整数自增后保存到文件私有数据中 */
    atomic64_inc(&v);
    dev1.sec = atomic_read(&v);
    mod_timer(&timer_test, jiffies_64 + msecs_to_jiffies(1000));
}


static int open_test(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;

    /* 添加定时器，不设置超时行为？ */
    /* timer_test.expires = jiffies_64 + msecs_to_jiffies(1000);*/
    add_timer(&timer_test);
    return 0;
}

static ssize_t read_test (struct file *file, char __user *buf, size_t size, loff_t *off)
{
    int ret;
    ret = copy_to_user(buf, &dev1.sec, sizeof(dev1.sec));
    if (ret != 0) {
        printk("copy_to_user error\n");
        return -1;
    }

    return 0;
}

static int release_test (struct inode *inode, struct file *file)
{
    del_timer(&timer_test);
    return 0;
}


static struct file_operations fops_test = {
    .owner = THIS_MODULE,  /* 指向本模块，避免模块的操作正使用时被卸载 */
    .open = open_test,
    .read = read_test,
    /*.write = write_test,*/
    .release = release_test,
};


static int __init atomic_init(void)
{
    int ret;

    /* 通过动态方式进行设备号注册 */
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "chrdev_name");
    if (ret < 0) {
        printk("alloc_chrdev_region is error: %d\n", ret);
    }
    printk("alloc_chrdev_region is ok");

    /* 通过宏获取主次设备号 */
    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    printk("major is %d, minor is %d\n", dev1.major, dev1.minor);

    /* 初始化设备结构体，并连接到设备操作结构体, 设置owner后添加字符设备 */
    cdev_init(&dev1.cdev_test, &fops_test);
    dev1.cdev_test.owner = THIS_MODULE;
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0) {
        printk("cdev_add is error: %d\n", ret);
        // todo: goto or return here?
    }

    /* 创建名为class_test的类，然后创建名称test的设备 */
    dev1.class_test = class_create(THIS_MODULE, "class_test");
    device_create(dev1.class_test, NULL, dev1.dev_num, NULL, "test"); /*设备的父节点和数据用NULL*/

    return 0;
}

static void __exit atomic_exit(void)
{
    /* 按顺序删除创建的设备，和类 */
    device_destroy(dev1.class_test, dev1.dev_num);
    class_destroy(dev1.class_test);

    /* 删除字符设备, 在释放字符驱动设备号之前. */
    cdev_del(&dev1.cdev_test);

    /* 释放字符驱动设备号 */
    unregister_chrdev_region(dev1.dev_num, 1); 

    printk("module exit\n");
}

module_init(atomic_init);
module_exit(atomic_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
