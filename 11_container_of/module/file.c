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

/* 定义文件私有数据的结构, 信息封装设备号，字符设备，类和设备，缓冲区等 */
struct device_test {
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

    /* 内核缓冲区 */
    char kbuf[32];
};

/* 定义文件私有数据结构体的变量，open时赋值，透传给read,write方法 */
static struct device_test dev1;
static struct device_test dev2;


static int cdev_test_open(struct inode *inode, struct file *file)
{
    /* 设置次设备号为0,1 */
    dev1.minor = 0;
    dev2.minor = 1;

    //file->private_data = &dev1;
    /* 通过inode->i_rdev找到私有数据结构体 */
    file->private_data = container_of(inode->i_cdev, struct device_test, cdev_test);

    printk("This is cdev_test_open \n");
    return 0;
}

static ssize_t cdev_test_read (struct file *file, char __user *buf, size_t size, loff_t *off)
{
    struct device_test *p = (struct device_test *)file->private_data;
    /* 内核空间的字符串数据缓冲，复制到用户空间返回 */
    /*char kbuf[32] = "This is cdev_test_read!";*/
    if (copy_to_user(buf, p->kbuf, strlen(p->kbuf)) != 0) {
        printk("copy_to_user error\r\n");
        return -1;
    }

    printk("This is cdev_test_read \r\n");
    return 0;
}

static ssize_t cdev_test_write (struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    struct device_test *p = (struct device_test *)file->private_data;
    /* 用户空间传来的数据复制到内核缓冲区 */
    /* char kbuf[32] = {0}; */
    if (p->minor == 0) {
        if (copy_from_user(p->kbuf, buf, size) != 0) {
            printk("copy_from_user 1 error\r\n");
            return -1;
        }
        printk("test device 1 kbuf is %s\r\n", p->kbuf);
    } else if (p->minor == 1) {
        if (copy_from_user(p->kbuf, buf, size) != 0) {
            printk("copy_from_user 2 error\r\n");
            return -1;
        }
        printk("test device 2 kbuf %s\r\n", p->kbuf);
    }
    
    return 0;
}

static int cdev_test_release (struct inode *inode, struct file *file)
{
    printk("This is cdev_test_realse \n");
    return 0;
}


static struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,  /* 指向本模块，避免模块的操作正使用时被卸载 */
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release,
};


static int __init chr_fops_init(void)
{
    int ret;

    /* 通过动态方式进行设备号注册2个设备号 */
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 2, "alloc_name");
    if (ret < 0) {
        printk("alloc_chrdev_region is error: %d\n", ret);
        // todo: goto or return?
    }
    printk("alloc_chrdev_region is ok %d\n", ret);

    /* 通过宏获取主次设备号 */
    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    printk("major is %d\n", dev1.major);
    printk("minor is %d\n", dev1.minor);

    /* 初始化设备结构体，并连接到设备操作结构体, 设置owner后添加字符设备 */
    dev1.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_ops);

    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0) {
        printk("cdev_add 1 error: %d\n", ret);
        // todo: goto or return here?
    }

    /* 创建名为class_test的类，然后创建名称test的设备 */
    dev1.class_test = class_create(THIS_MODULE, "test1");
    dev1.device = device_create(dev1.class_test, NULL, dev1.dev_num, NULL, "test1"); /*设备的父节点和数据用NULL*/

    dev2.dev_num = dev1.dev_num + 1;
    dev2.major = MAJOR(dev2.dev_num);
    dev2.minor = MINOR(dev2.dev_num);
    printk("major is %d\n", dev2.major);
    printk("minor is %d\n", dev2.minor);

    dev2.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev2.cdev_test, &cdev_test_ops);

    ret = cdev_add(&dev2.cdev_test, dev2.dev_num, 1);
    if (ret < 0) {
        printk("cdev_add 2 error\n");
        // todo: goto or return here?
    }

    dev2.class_test = class_create(THIS_MODULE, "test2");
    dev2.device = device_create(dev2.class_test, NULL, dev2.dev_num, NULL, "test2");

    return 0;
}

static void __exit chr_fops_exit(void)
{
    /* 删除字符设备, 在释放字符驱动设备号之前. */
    cdev_del(&dev1.cdev_test);
    cdev_del(&dev2.cdev_test);

    /* 释放字符驱动设备号 */
    unregister_chrdev_region(dev1.dev_num, 1); 
    unregister_chrdev_region(dev2.dev_num, 1); 

    /* 按顺序删除创建的设备，和类 */
    device_destroy(dev1.class_test, dev1.dev_num);
    class_destroy(dev1.class_test);

    device_destroy(dev2.class_test, dev2.dev_num);
    class_destroy(dev2.class_test);

    printk("module exit\n");
}

module_init(chr_fops_init);
module_exit(chr_fops_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
