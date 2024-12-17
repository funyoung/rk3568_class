#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/io.h>

struct device_test {
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;

    char kbuf[32];
    unsigned int *vir_gpio_dr;
};


static struct device_test dev1;

static int cdev_test_open(struct inode *inode, struct file *file)
{
    file->private_data = &dev1;
    printk("this is cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;
    if (copy_from_user(test_dev->kbuf, buf, size) != 0)
    {
        printk("copy_from_user error\n");
        return -1;
    }

    if (test_dev->kbuf[0] == 1) {
        *(test_dev->vir_gpio_dr) = 0x8000c040;
        printk("test_dev->kbuf[0] is %d\n", test_dev->kbuf[0]);
    } else if (test_dev->kbuf[0] == 0) {
        *(test_dev->vir_gpio_dr) = 0x80004040;
        printk("test_dev->kbuf[0] is %d\n", test_dev->kbuf[0]);
    }
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    struct device_test *test_dev = (struct device_test *)file->private_data;
    if (copy_to_user(buf, test_dev->kbuf, strlen(test_dev->kbuf)) != 0)
    {
        printk("copy_to_user error\n");
        return -1;
    }

    printk("this is cdev_test_read\n");
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file)
{
    printk("This is cdev_test_release\n");
    return 0;
}

/* 设备操作函数 */
struct file_operations cdev_test_fops = {
    .owner = THIS_MODULE,
    .open = cdev_test_open,
    .read = cdev_test_read,
    .write = cdev_test_write,
    .release = cdev_test_release,
};

/* 平台设备探索函数，匹配上后注册字符设备驱动 */
static int my_platform_probe(struct platform_device *pdev)
{
    struct resource *res_mem;
    int ret;

    printk(KERN_INFO "Probing platform device\n");

    res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res_mem) {
        dev_err(&pdev->dev, "Failed to get memory resource\n");
        return -ENODEV;
    } 

    /* 创建字符设备驱动 */
    /* 1. 动态分配字符设备号 */
    ret = alloc_chrdev_region(&dev1.dev_num, 0, 1, "alloc_name");
    if (ret < 0) goto err_chrdev;

    printk("alloc_chrdev_region is ok\n");

    dev1.major = MAJOR(dev1.dev_num);
    dev1.minor = MINOR(dev1.dev_num);
    printk("major is %d, minor is %d\n", dev1.major, dev1.minor);

    /* 2. 初始化cdev */
    dev1.cdev_test.owner = THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_fops);

    /* 3. 添加cdev，字符设备注册到内核完成 */
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0) goto err_chr_add;

    /* 4. 创建类 */
    dev1.class = class_create(THIS_MODULE, "test");
    if (IS_ERR(dev1.class)) {
        ret = PTR_ERR(dev1.class);
        goto err_class_create;
    }

    /* 5. 创建设备 */
    dev1.device = device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
    if (IS_ERR(dev1.device)) {
        ret = PTR_ERR(dev1.device);
        goto err_device_create;
    }


    /* 把platform设备物理地址转化到虚拟地址 */
    dev1.vir_gpio_dr = ioremap(res_mem->start, 4);
    if (IS_ERR(dev1.vir_gpio_dr)) {
        ret = PTR_ERR(dev1.vir_gpio_dr);
        goto err_ioremap;
    }

    printk("probe ok \n");
    
    return 0;

err_ioremap:
    iounmap(dev1.vir_gpio_dr);
err_device_create:
    class_destroy(dev1.class);  /* 删除类 */
err_class_create:
    cdev_del(&dev1.cdev_test); /* 删除cdev */
err_chr_add:
    unregister_chrdev_region(dev1.dev_num, 1); /* 注销设备号 */
err_chrdev:
    printk("error %d\n", ret);
    return ret;
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

    printk(KERN_INFO "Platform driver initialized\n");
    return 0;
}

static void __exit my_platform_driver_exit(void)
{
    /* 注销字符设备 */
    unregister_chrdev_region(dev1.dev_num, 1); /* 注销设备号 */
    cdev_del(&dev1.cdev_test); /* 删除cdev */
    device_destroy(dev1.class, dev1.dev_num); /* 通过类和设备号删除设备 */
    class_destroy(dev1.class);  /* 删除类 */

    /* 注销平台驱动 */
    platform_driver_unregister(&my_platform_driver); 
    printk(KERN_INFO "Platform driver exited\n");
}

module_init(my_platform_driver_init);
module_exit(my_platform_driver_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
