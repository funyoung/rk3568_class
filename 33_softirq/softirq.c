#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
/*#include <linux/delay.h>*/

/* GPIO3_A5: 3*32 + 0 * 8 + 5 = 101 */
#define GPIO_PIN 101

static int irq;

/* 定义tasklet结构体和处理函数 */
void testsoft_func(struct softirq_action *softirq_action)
{
    printk(KERN_INFO "this is testsoft_func\n");
    /*msleep(3000);*/
}


static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "Interrupt occurs: %d\n", irq);
    printk(KERN_INFO "this is interrupt handler.\n");
    raise_softirq(TEST_SOFTIRQ);
    return IRQ_RETVAL(IRQ_HANDLED);
}

/* 引脚映射成中断号，请求中断 */
static int __init soft_irq_init(void)
{

    printk(KERN_INFO "Initializing GPIO Interrupt Driver\n");
    irq = gpio_to_irq(GPIO_PIN);
    printk(KERN_INFO "GPIO %d map to irq %d\n", GPIO_PIN, irq);

    if (request_irq(irq, gpio_irq_handler, IRQF_TRIGGER_RISING, "irq_test",  NULL) != 0) {
        printk(KERN_INFO "Failed to request IRQ %d\n", irq);
        gpio_free(GPIO_PIN);
        return -ENODEV;
    }

    open_softirq(TEST_SOFTIRQ, testsoft_func);

    return 0;
}

static void __exit soft_irq_exit(void)
{
    free_irq(irq, NULL);

    printk(KERN_INFO "GPIO Interrupt Driver exit successully.\n");
}

module_init(soft_irq_init);
module_exit(soft_irq_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
