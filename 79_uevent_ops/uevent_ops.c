#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/configfs.h>
#include <linux/kernel.h>
#include <linux/kobject.h>

struct kobject *mykobject01, *mykobject02;
struct kset *mykset;
struct kobj_type mytype;

const char *myname(struct kset *kset, struct kobject *kobj)
{
    return "my_kset";
}

int myuevent(struct kset *kset, struct kobject *kobj, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "MYDEVICE=%s", "TOPEET");
    return 0;
}

int myfilter(struct kset *kset, struct kobject *kobj)
{
    if (strcmp(kobj->name, "mykobject01") == 0) {
        return 0; /* 不过滤 */
    } else {
        return 1; /* 过滤 */
    }
}
struct kset_uevent_ops my_uevent_ops = {
    .filter = myfilter,
    .uevent = myuevent,
    .name = myname,
};

static int __init myuevent_init(void)
{
    int ret;

    mykset = kset_create_and_add("mykset", &my_uevent_ops, NULL);
    mykobject01 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    mykobject01->kset = mykset;

    /* 初始化并添加kobject到kset */
    ret = kobject_init_and_add(mykobject01, &mytype, NULL, "%s", "mykobject01");
    /* todo: error probe */

    mykobject02 = kzalloc(sizeof(struct kobject), GFP_KERNEL);
    mykobject02->kset = mykset;

    /* 初始化并添加kobject到kset */
    ret = kobject_init_and_add(mykobject02, &mytype, NULL, "%s", "mykobject02");
    /* todo: error probe */

    /* 触发一个uevent事件，表示Kobject01的属性发生了变化 */
    ret = kobject_uevent(mykobject01, KOBJ_CHANGE);

    /* 触发一个uevent事件，表示Kobject02被添加 */
    ret = kobject_uevent(mykobject02, KOBJ_ADD);

    return 0;
}

static void __exit myuevent_exit(void)
{
    kobject_put(mykobject01);
    kobject_put(mykobject02);
    kset_unregister(mykset);
}


module_init(myuevent_init);
module_exit(myuevent_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
