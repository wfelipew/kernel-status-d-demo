#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/sched.h>


/*Set file operations handle functions */



static int statusd_init(void){
	printk(KERN_ALERT "Statusd_demo started \n");

	/*Create character device*/
	
	return 0;
}


static void statusd_exit(void){
	printk(KERN_INFO "Statusd_demo stopped");
}


module_init(statusd_init);
module_exit(statusd_exit);

MODULE_AUTHOR("William Felipe Welter");
MODULE_LICENSE("BSD");
MODULE_DESCRIPTION("Linux kernel module that create a character device to simulate/force uniterruptble sleep state for testing purpose");
