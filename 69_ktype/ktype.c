#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

/* 定义kobject指针变量 */
struct kobject *mykobject03;

/* 定义含释放kobject类型函数的kobj_type结构体变量 */
static void dynamic_kobj_release(struct kobject *kobj)
{
    printk("kobject: (%p): %s\n", kobj, __func__);
    kfree(kobj);
}
struct kobj_type mytype = {
    .release = dynamic_kobj_release,
};

static int __init mykobj_init(void)
{
    int ret;

    /* 创建kobject方法2，创建/sys/下对象3 */
    mykobject03 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    ret = kobject_init_and_add(mykobject03, &mytype, NULL, "%s", "mykobject03");
    if (ret < 0) {
        printk("mykobj_init failed\n");
        return -1;
    }

    printk("kobject_init_and_add result: %d, kobject: %p\n", ret, mykobject03);
   
    return 0;
}

static void __exit mykobj_exit(void)
{
    /* 释放先前创建的kobject对象 */
    printk("%s: before kobject_put(%p)\n", __func__, mykobject03);
    kobject_put(mykobject03);
    printk("%s: AFTER kobject_put(%p)\n", __func__, mykobject03);
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
