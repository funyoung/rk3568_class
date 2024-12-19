#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

/* 定义3个kobject指针变量，和描述kobject类型的kobj_type结构体变量 */
struct kobject *mykobject01, *mykobject02, *mykobject03;
struct kobj_type mytype;

/* 打印内核对象的引用计数 */
static void print_kref(struct kobject *kobj)
{
    printk("%s kref is %d\n", kobj->name, kobj->kref.refcount.refs.counter);
}

static void print_kref_all(void) {
    print_kref(mykobject01);
    print_kref(mykobject02);
    print_kref(mykobject03);
}

static void print_func_enter(void)
{
    printk("%s: enter\n", __func__);
}

static void print_func_exit(void)
{
    printk("%s: exit\n", __func__);
}

static int __init mykobj_init(void)
{
    int ret;

    /* 创建Kobject方法1，创建/sys/下对象1及其下对象2 */
    mykobject01 = kobject_create_and_add("mykobject01", NULL);
    print_kref(mykobject01);
    mykobject02 = kobject_create_and_add("mykobject02", mykobject01);
    print_kref(mykobject01);
    print_kref(mykobject02);

    /* 创建kobject方法2，创建/sys/下对象3 */
    mykobject03 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    ret = kobject_init_and_add(mykobject03, &mytype, NULL, "%s", "mykobject03");
    if (ret < 0) {
        printk("mykobj_init failed\n");
        return -1;
    }
    print_kref(mykobject03);
   
    return 0;
}

static void __exit mykobj_exit(void)
{
    print_func_enter();
    /* 释放先前创建的kobject对象 */
    print_kref_all();
    kobject_put(mykobject01);
    print_kref_all();
    kobject_put(mykobject02);
    print_kref_all();
    kobject_put(mykobject03);
    print_kref_all();
    print_func_exit();
}

module_init(mykobj_init);
module_exit(mykobj_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
