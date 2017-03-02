/* Basic Linux kernel driver template */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
int driver_init(void);
int driver_exit(void);
module_init(driver_init);
module_exit(driver_exit);

/* Loader */
int driver_init(void) {
	printk("Module loaded\n");
	return 0;
}

/* Exit */
void module_exit(void) {
	printk("Module unloaded\n");
}
