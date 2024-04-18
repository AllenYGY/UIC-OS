#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hash.h>
#include <linux/gcd.h>
#include <asm/param.h>
#include <linux/jiffies.h>

long int old_jiffies;

/* This function is called when the module is loaded. */
int simple_init(void)
{

    printk(KERN_INFO "Loading Kernel Module∖n");

    printk(KERN_INFO "%lu\n", GOLDEN_RATIO_PRIME);
    printk(KERN_INFO "%d, %ld\n", HZ, jiffies);
    old_jiffies = jiffies;

    return 0;
}

/* This function is called when the module is removed. */

void simple_exit(void)
{

    printk(KERN_INFO "Removing Kernel Module∖n");

    printk(KERN_INFO "%ld\n", gcd(3300, 24));
    printk(KERN_INFO "%ld\n", jiffies);
    printk(KERN_INFO "%ld\n", (jiffies - old_jiffies) / HZ);
}
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("YJY");