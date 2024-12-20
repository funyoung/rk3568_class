#include <linux/init.h>
#include <linux/module.h>

int num = 10; /* 定义参数 */
EXPORT_SYMBOL(num); /* 导出参数num */

int add(int a, int b) /* 定义数学函数add()，用来实现加法，不能是static? */
{
	return a + b;
}
EXPORT_SYMBOL(add); /* 导出参数学函数add() */

static int __init math_init(void) /* 驱动入口函数 */
{
	printk("math_module init\n");
	return 0;
}

static void __exit math_exit(void) /* 驱动出口函数 */
{
	printk("math_module exit\n");
}

module_init(math_init);
module_exit(math_exit);



MODULE_LICENSE("GPL"); /* 同意GPL开源协议 */
MODULE_AUTHOR("YF");   /* 作者信息 */
MODULE_VERSION("V1.0");/* 版本信息 */
