#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

struct kobject *mykobject01;
struct kset *mykset;
struct kobj_type mytype;

static int __init myuevent_init(void)
{
    int ret;

    mykset = kset_create_and_add("mykset", NULL, NULL);
    mykobject01 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    mykobject01->kset = mykset;

    /* 初始化并添加kobject到kset */
    ret = kobject_init_and_add(mykobject01, &mytype, NULL, "%s", "mykobject01");
    /* todo: error probe */

    /* 触发一个uevent事件，表示Kobject的属性发生了变化 */
    ret = kobject_uevent(mykobject01, KOBJ_CHANGE);

    return 0;
}

static void __exit myuevent_exit(void)
{
    kobject_put(mykobject01);
    kset_unregister(mykset);
}


module_init(myuevent_init);
module_exit(myuevent_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
