#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

/* 定义kobject指针变量，kset指针变量和描述kobject类型的kobj_type结构体变量 */
struct kobject *mykobject01, *mykobject02;
struct kset *mykset;
struct kobj_type mytype;

static int __init mykobj_init(void)
{
    int ret;

    /* 创建"mykset"，父object和属性都为NULL */
    mykset = kset_create_and_add("mykset", NULL, NULL);

    /* 创建kobject方法2，创建内核对象并设置kset属性 */
    mykobject01 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    mykobject01->kset = mykset;
    ret = kobject_init_and_add(mykobject01, &mytype, NULL, "%s", "mykobject01");
    if (ret < 0) {
        printk("kobject_init_and_add 01 failed\n");
        return -1;
    }

    mykobject02 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    mykobject02->kset = mykset;
    ret = kobject_init_and_add(mykobject02, &mytype, NULL, "%s", "mykobject02");
    if (ret < 0) {
        printk("kobject_init_and_add 02 failed\n");
        return -1;
    }
  
    return 0;
}

static void __exit mykobj_exit(void)
{
    /* 释放先前创建的kobject对象 */
    /* kobject_put(mkset); */
    kobject_put(mykobject01);
    kobject_put(mykobject02);
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
