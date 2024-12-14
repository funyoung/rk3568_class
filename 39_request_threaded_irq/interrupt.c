#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

/* GPIO3_A5: 3*32 + 0 * 8 + 5 = 101 */
#define GPIO_PIN 101
static int irq_num;

/* 中断下半部，线程化中断处理函数 */
static irqreturn_t test_work(int irq, void *args)
{
    msleep(1000);
    printk("This is test_work!\n");
    return IRQ_RETVAL(IRQ_HANDLED);
}
/* 中断上半部，中断处理函数 */
static irqreturn_t test_interrupt(int irq, void *dev_id)
{
    printk(KERN_INFO "Interrupt occurs: %d\n", irq);
    printk(KERN_INFO "this is interrupt handler.\n");
    /* 返回值，内核唤醒执行下半部 */
    return IRQ_WAKE_THREAD;
}

/* 引脚映射成中断号，请求中断 */
static int __init interrupt_init(void)
{
    int ret;

    printk(KERN_INFO "Initializing GPIO Interrupt Driver\n");
    irq_num = gpio_to_irq(GPIO_PIN);
    printk(KERN_INFO "GPIO %d map to irq %d\n", GPIO_PIN, irq_num);

    /* 请求并注册线程化中断处理函数 */
    ret = request_threaded_irq(irq_num, test_interrupt, test_work, IRQF_TRIGGER_RISING, "test", NULL);
    if (ret < 0) {
        printk(KERN_INFO "Failed to request IRQ %d\n", irq_num);
        gpio_free(GPIO_PIN);
        return -ENODEV;
    }

    return 0;
}

static void __exit interrupt_exit(void)
{
    irq_num = gpio_to_irq(GPIO_PIN);
    free_irq(irq_num, NULL);

    printk(KERN_INFO "GPIO Interrupt Driver exit successully.\n");
}

module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
