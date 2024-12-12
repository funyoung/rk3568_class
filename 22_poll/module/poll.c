#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/wait.h>
#include <linux/poll.h>

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

    int flag;  /* 标志位 */
};

/* 定义文件私有数据结构体的变量，open时赋值，透传给read,write方法 */
static struct device_test dev1;

/* 定义并初始化等待队列头 */
DECLARE_WAIT_QUEUE_HEAD(read_wq);


static int cdev_test_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;
    printk("This is cdev_test_open \n");
    return 0;
}

static ssize_t cdev_test_read (struct file *file, char __user *buf, size_t size, loff_t *off)
{
    struct device_test *p = (struct device_test *)file->private_data;

    if (file->f_flags & O_NONBLOCK) {
        if (p->flag != 1) return -EAGAIN;
    }

    /* 可中断的阻塞，进程休眠 */
    wait_event_interruptible(read_wq, p->flag);

    /* 内核空间的字符串数据缓冲，复制到用户空间返回 */
    /*char kbuf[32] = "This is cdev_test_read!";*/
    /* todo: p->kbuf没有初始化??? */
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
    if (copy_from_user(p->kbuf, buf, size) != 0) {
        printk("copy_from_user error\r\n");
        return -1;
    }

    p->flag = 1;
    wake_up_interruptible(&read_wq);

    printk("This is cdev_test_write \r\n");
    return 0;
}

static int cdev_test_release (struct inode *inode, struct file *file)
{
    printk("This is cdev_test_realse \n");
    return 0;
}

__poll_t cdev_test_poll (struct file *file, struct poll_table_struct *pts)
{
    __poll_t mask;
    struct device_test *p = (struct device_test *)file->private_data;
    mask = 0;
    /* 阻塞应用 */
    poll_wait(file, &read_wq, pts);
    if (p->flag == 1) {
        mask |= POLLIN;
    }

    return mask;
}


static struct file_operations cdev_test_ops = {
    .owner = THIS_MODULE,  /* 指向本模块，避免模块的操作正使用时被卸载 */
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release,
    .poll = cdev_test_poll,
};


static int __init chr_fops_init(void)
{
    int ret;

    /* 通过动态方式进行设备号注册 */
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "alloc_name");
    if (ret < 0) {
        goto err_chrdev;
    }
    printk("alloc_chrdev_region is ok %d\n", ret);

    /* 通过宏获取主次设备号 */
    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    printk("major is %d\n", dev1.major);
    printk("minor is %d\n", dev1.minor);

    /* 初始化设备结构体，并连接到设备操作结构体, 设置owner后添加字符设备 */
    cdev_init(&dev1.cdev_test, &cdev_test_ops);
    dev1.cdev_test.owner = THIS_MODULE;
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0) {
        goto err_chr_add;
        // todo: goto or return here?
    }
    printk("cdev_add is ok\n");

    /* 创建名为class_test的类，然后创建名称test的设备 */
    dev1.class_test = class_create(THIS_MODULE, "class_test");
    if (IS_ERR(dev1.class_test)) {
        ret = PTR_ERR(dev1.class_test);
        goto err_class_create;
    } 
    
    /*设备的父节点和数据用NULL*/
    dev1.device = device_create(dev1.class_test, NULL, dev1.dev_num, NULL, "test"); 
    if (IS_ERR(dev1.device)) {
        ret = PTR_ERR(dev1.device);
        goto err_device_create;
    }

    return 0;

err_device_create:
    class_destroy(dev1.class_test);
    printk("device_create error handle: %d\n", ret);

err_class_create:
    cdev_del(&dev1.cdev_test);
    printk("class_create error: %d\n", ret);

err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1);
    printk("cdev_add is error: %d\n", ret);

err_chrdev:
    printk("alloc_chrdev_region is error: %d\n", ret);
    return ret;
}

static void __exit chr_fops_exit(void)
{
    /* 删除字符设备, 在释放字符驱动设备号之前. */
    cdev_del(&dev1.cdev_test);

    /* 释放字符驱动设备号 */
    unregister_chrdev_region(dev1.dev_num, 1); 

    /* 按顺序删除创建的设备，和类 */
    device_destroy(dev1.class_test, dev1.dev_num);
    class_destroy(dev1.class_test);
    printk("module exit\n");
}

module_init(chr_fops_init);
module_exit(chr_fops_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
