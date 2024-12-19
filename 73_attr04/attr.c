#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

/* 定义kobject指针变量*/
struct kobject *mykobject01;

ssize_t show_myvalue1(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    ssize_t count;
    count = sprintf(buf, "show_myvalue1\n");
    return count;
}
ssize_t store_myvalue1(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    count = printk("buf is %s\n", buf);
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
    count = printk("buf is %s\n", buf);
    return count;
}
struct kobj_attribute value1 = __ATTR(value1, 0664, show_myvalue1, store_myvalue1);
struct kobj_attribute value2 = __ATTR(value2, 0664, show_myvalue2, store_myvalue2);

/* 属性组定义 */
struct attribute *attr[] = {
    &value1.attr,
    &value2.attr,
    NULL,
};
const struct attribute_group my_attr_group = {
    .name = "myattr",
    .attrs = attr,
};

static int __init mykobj_init(void)
{
    int ret;

    /* 创建Kobject方法1，创建/sys/下对象1及其下对象2 */
    mykobject01 = kobject_create_and_add("mykobject01", NULL);

    /*ret = sysfs_create_file(mykobject01, &value1.attr);
    ret = sysfs_create_file(mykobject01, &value2.attr);*/
    ret = sysfs_create_group(mykobject01, &my_attr_group);

    /* todo: error probe */

    return ret;
}

static void __exit mykobj_exit(void)
{
    /* 释放先前创建的kobject对象 */
    kobject_put(mykobject01);
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
