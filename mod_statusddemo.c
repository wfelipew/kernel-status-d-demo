#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/sched.h>


/*Set file operations handle functions */
int statusd_open(struct inode *inode, struct file *fileptr){
	return 1;
}

int statusd_release(struct inode *inode, struct file *fileptr){
	return 1;
}

ssize_t statusd_read(struct file *fileptr, char *buf, size_t count, loff_t *f_pos){
	return 1;
}

ssize_t statusd_write( struct file *fileptr, const char *buf, size_t count, loff_t *f_pos){
	return 1;
}

struct file_operations  statusd_fops= {
    .read = statusd_read,
    .write = statusd_write,
    .open = statusd_open,
    .release = statusd_release
};

int major;

static int statusd_init(void){
	printk(KERN_ALERT "Statusd_demo started \n");

	/*Create character device with dynamic Major number*/
	major = register_chrdev(0, "statusd_demo", &statusd_fops); 
	if( major < 0 ){
		printk(KERN_ALERT "Failed to create chracter device");
		return -1;
	}

	/*Allocate memory*/
	
	return 0;
}


static void statusd_exit(void){
	printk(KERN_INFO "Statusd_demo stopped");
	/*Unregister character device*/
	unregister_chrdev(major,"statusd_demo");
}


module_init(statusd_init);
module_exit(statusd_exit);

MODULE_AUTHOR("William Felipe Welter");
MODULE_LICENSE("BSD");
MODULE_DESCRIPTION("Linux kernel module that create a character device to simulate/force uniterruptble sleep state for testing purpose");
