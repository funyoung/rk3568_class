#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

/* 自定义包含kobject和两个整形属性之的结构体, 及其对象 */
struct mykobj
{
    struct kobject kobj;
    int value1;
    int value2;
};

/* 定义kobject指针变量 */
struct mykobj *mykobject01;

/**/

/* 定义含释放kobject类型函数的kobj_type结构体变量 */
static void dynamic_kobj_release(struct kobject *kobj)
{
    struct mykobj *mykobject01;
    printk("kobject: (%p): %s\n", kobj, __func__);
    mykobject01 = container_of(kobj, struct mykobj, kobj);
    kfree(mykobject01);
}

/* 自定义属性对象数组 */
/* 自定义属性value1和value2的读写属性值函数*/
ssize_t show_myvalue1(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    ssize_t count;
    count = sprintf(buf, "show_myvalue1\n");
    return count;
}
ssize_t store_myvalue1(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    printk("buf is %s\n", buf);
    return count;
}
ssize_t show_myvalue2(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    ssize_t count;
    count = sprintf(buf, "show_myvalue2\n");
    return count;
}
ssize_t store_myvalue2(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    printk("buf is %s\n", buf);
    return count;
}
struct kobj_attribute value1 = __ATTR(value1, 0664, show_myvalue1, store_myvalue1);
struct kobj_attribute value2 = __ATTR(value2, 0664, show_myvalue2, store_myvalue2);
struct attribute *myattr[] = {
    &value1.attr,
    &value2.attr,
    NULL,
};

/* 自定义虚文件系统操作结构体sysfs_ops，包含show和store函数指针 */
/* 读属性值的show函数 */
ssize_t myshow(struct kobject *kobj, struct attribute *attr, char *buf)
{
    ssize_t count;
    struct kobj_attribute *kobj_attr;

    kobj_attr = container_of(attr, struct kobj_attribute, attr);
    count = kobj_attr->show(kobj, kobj_attr, buf);
    return count;
}
/* 写属性值的store函数 */
ssize_t mystore(struct kobject *kobj, struct attribute *attr, const char *buf, size_t size)
{
    struct kobj_attribute *kobj_attr;

    kobj_attr = container_of(attr, struct kobj_attribute, attr);
    
    return kobj_attr->store(kobj, kobj_attr, buf, size);
}
struct sysfs_ops myops = {
    .show = myshow,
    .store = mystore,
};

/* 自定义kobj_type包含释放函数，默认属性和虚拟文件系统操作结构体sysfs_ops */
struct kobj_type mytype = {
    .release = dynamic_kobj_release,
    .default_attrs = myattr,
    .sysfs_ops = &myops,
};

static int __init mykobj_init(void)
{
    int ret;

    /* 创建kobject方法2，创建/sys/下自定义的内核对象, 含初始化的属性值 */
    mykobject01 = kzalloc(sizeof(struct mykobj), GFP_KERNEL);
    mykobject01->value1 = 11;
    mykobject01->value2 = 22;

    ret = kobject_init_and_add(&mykobject01->kobj, &mytype, NULL, "%s", "mykobject01");
    if (ret < 0) {
        printk("mykobj_init failed\n");
        return -1;
    }

    printk("kobject_init_and_add result: %d, kobject: %p\n", ret, mykobject01);
   
    return 0;
}

static void __exit mykobj_exit(void)
{
    /* 释放先前创建的kobject对象 */
    printk("%s: before kobject_put(%p)\n", __func__, mykobject01);
    kobject_put(&mykobject01->kobj);
    printk("%s: AFTER kobject_put(%p)\n", __func__, mykobject01);
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
