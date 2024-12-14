#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

/* GPIO3_A5: 3*32 + 0 * 8 + 5 = 101 */
#define GPIO_PIN 101

struct work_data {
    struct work_struct test_work;
    int a;
    int b;
};

/* 自定义工作队列和工作项和中断下文工作项处理函数 */
static struct workqueue_struct *test_workqueue;
static struct work_data test_workqueue_work;
static void test_work(struct work_struct *work)
{
    struct work_data *pdata;
    pdata = container_of(work, struct work_data, test_work);
    printk(KERN_INFO "work queue arg a = %d\n", pdata->a);
    printk(KERN_INFO "work queue arg b = %d\n", pdata->b);
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "Interrupt occurs: %d\n", irq);
    printk(KERN_INFO "this is interrupt handler.\n");

    /* 提交工作项到自定义工作队列 */
    queue_work(test_workqueue, &test_workqueue_work.test_work);
    return IRQ_RETVAL(IRQ_HANDLED);
}

/* 引脚映射成中断号，请求中断 */
static int __init interrupt_init(void)
{
    int irq_num;

    printk(KERN_INFO "Initializing GPIO Interrupt Driver\n");
    irq_num = gpio_to_irq(GPIO_PIN);
    printk(KERN_INFO "GPIO %d map to irq %d\n", GPIO_PIN, irq_num);

    if (request_irq(irq_num, gpio_irq_handler, IRQF_TRIGGER_RISING, "irq_test",  NULL) != 0) {
        printk(KERN_INFO "Failed to request IRQ %d\n", irq_num);
        gpio_free(GPIO_PIN);
        return -ENODEV;
    }

    /* 创建工作队列并初始化工作项 */
    test_workqueue = create_workqueue("test_workqueue");
    INIT_WORK(&test_workqueue_work.test_work, test_work);
    test_workqueue_work.a = 2024;
    test_workqueue_work.b = 12;


    return 0;
}

static void __exit interrupt_exit(void)
{
    int irq_num;
    irq_num = gpio_to_irq(GPIO_PIN);
    free_irq(irq_num, NULL);

    /* 取消工作项，刷新队列，销毁工作队列*/
    cancel_work_sync(&test_workqueue_work.test_work);
    flush_workqueue(test_workqueue);
    destroy_workqueue(test_workqueue);

    printk(KERN_INFO "GPIO Interrupt Driver exit successully.\n");
}

module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
