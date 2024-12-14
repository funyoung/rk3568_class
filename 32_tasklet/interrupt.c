#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
/* #include <linux/delay.h> */

/* GPIO3_A5: 3*32 + 0 * 8 + 5 = 101 */
#define GPIO_PIN 101

static int irq;

/* 定义tasklet结构体和处理函数 */
struct tasklet_struct mytasklet;
void mytasklet_func(unsigned long data)
{
    printk(KERN_INFO "tasklet data is %ld\n", data);
    /* tasklet处理函数不能阻塞 */
    /* msleep(3000); */
}


static irqreturn_t gpio_irq_handler(int irq, void *dev_id)
{
    printk(KERN_INFO "Interrupt occurs: %d\n", irq);
    printk(KERN_INFO "this is interrupt handler.\n");
    /* 调度tasklet */
    tasklet_schedule(&mytasklet);
    return IRQ_RETVAL(IRQ_HANDLED);
}

/* 引脚映射成中断号，请求中断 */
static int __init interrupt_irq_init(void)
{

    printk(KERN_INFO "Initializing GPIO Interrupt Driver\n");
    irq = gpio_to_irq(GPIO_PIN);
    printk(KERN_INFO "GPIO %d map to irq %d\n", GPIO_PIN, irq);

    if (request_irq(irq, gpio_irq_handler, IRQF_TRIGGER_RISING, "irq_test",  NULL) != 0) {
        printk(KERN_INFO "Failed to request IRQ %d\n", irq);
        gpio_free(GPIO_PIN);
        return -ENODEV;
    }

    /* 初始化tasklet, 传数据1 */
    tasklet_init(&mytasklet, mytasklet_func, 1);

    return 0;
}

static void __exit interrupt_irq_exit(void)
{
    free_irq(irq, NULL);

    /* 使能tasklet并销毁它 */
    tasklet_enable(&mytasklet);
    tasklet_kill(&mytasklet);

    printk(KERN_INFO "GPIO Interrupt Driver exit successully.\n");
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("YF");
MODULE_VERSION("V1.0");
