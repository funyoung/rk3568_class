#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

/* GPIO3_A5: 3*32 + 0 * 8 + 5 = 101 */
#define GPIO_PIN 101

/* 定义工作项和处理函数 */
struct work_struct test_workqueue;
void test_work(struct work_struct *work)
{
    msleep(1000);
    printk("This is test_work!\n");
}

static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "Interrupt occurs: %d\n", irq);
    printk(KERN_INFO "this is interrupt handler.\n");
    schedule_work(&test_workqueue);
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

    /* 初始化工作项 */
    INIT_WORK(&test_workqueue, test_work);

    return 0;
}

static void __exit interrupt_exit(void)
{
    int irq_num;
    irq_num = gpio_to_irq(GPIO_PIN);
    free_irq(irq_num, NULL);

    /* 取消工作项? */
    cancel_work_sync(&test_workqueue);

    printk(KERN_INFO "GPIO Interrupt Driver exit successully.\n");
}

module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
