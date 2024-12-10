#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

/** misc设备的主设备号固定为10，以此设备号区分
  * 在linux虚拟文件系统/proc/devices里列举了系统当前所有设备号
  * #cat /proc/devices
  */


/* 定义cdev设备结构体和file_operations操作结构体 */
static struct file_operations misc_fops = {
    .owner = THIS_MODULE,  /* 指向本模块，避免模块的操作正使用时被卸载 */
};

/* 杂项设备结构体，动态申请次设备号，名为"test"， 并和文件操作结构体关联 */
static struct miscdevice misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "test",
    .fops = &misc_fops,
};


static int __init misc_init(void)
{
    int ret;

    /* 注册杂项设备 */
    ret = misc_register(&misc_dev);
    if (ret < 0) {
        printk("misc register is error: %d\n", ret);
    }
    printk("misc register is succeed \n");


    return 0;
}

static void __exit misc_exit(void)
{
    /* 删除杂项设备 */
    misc_deregister(&misc_dev);

    printk("misc goodbye!\n");
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
