#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

/* 定义3个kobject指针变量，和描述kobject类型的kobj_type结构体变量 */
struct kobject *mykobject01, *mykobject02, *mykobject03;
struct kobj_type mytype;

static int __init mykobj_init(void)
{
    int ret;

    /* 创建Kobject方法1，创建/sys/下对象1及其下对象2 */
    mykobject01 = kobject_create_and_add("mykobject01", NULL);
    mykobject02 = kobject_create_and_add("mykobject02", mykobject01);

    /* 创建kobject方法2，创建/sys/下对象3 */
    mykobject03 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    ret = kobject_init_and_add(mykobject03, &mytype, NULL, "%s", "mykobject03");
    if (ret < 0) {
        printk("mykobj_init failed\n");
        return -1;
    }
   
    return 0;
}

static void __exit mykobj_exit(void)
{
    /* 释放先前创建的kobject对象 */
    kobject_put(mykobject01);
    kobject_put(mykobject02);
    kobject_put(mykobject03);
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
