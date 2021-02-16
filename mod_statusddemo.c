#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/proc_fs.h>
#include <linux/sysctl.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/wait.h>

static DECLARE_WAIT_QUEUE_HEAD(queue);
int major;
static struct class *myclass = NULL;
static struct cdev statusd_dev;
static int lock=0;



/*Set file operations handle functions */
int statusd_open(struct inode *inode, struct file *fileptr){
	return 0;
}

int statusd_release(struct inode *inode, struct file *fileptr){
	return 0;
}

ssize_t statusd_read(struct file *fileptr, char *buf, size_t count, loff_t *f_pos){
	/*Put the reader process in uninterruptable state AKA D Status*/
	lock=0;
	wait_event(queue,lock!=0);
	return 0;
}

ssize_t statusd_write( struct file *fileptr, const char *buf, size_t count, loff_t *f_pos){
	/*Wakeup the reader process*/
	lock=1;
	wake_up(&queue);
	/*Emulate a fake write*/
	return 1;
}

struct file_operations  statusd_fops= {
    .read = statusd_read,
    .write = statusd_write,
    .open = statusd_open,
    .release = statusd_release
};


static int statusd_init(void){
	printk(KERN_ALERT "Statusd_demo started \n");

	/*Allocate Major and Minor number region for this device*/
	if(alloc_chrdev_region(&major,0,1,"statusd_demo")  < 0 ){
		printk(KERN_ALERT "Failed to create chracter device");
		return -1;
	}
	printk("Character device created with  %d major number",major);

	/*Create device class*/
	myclass = class_create(THIS_MODULE,"statusd");
	if( myclass == NULL ){
		printk(KERN_ALERT "Failed to create device class");
                return -1;
	}	
        printk("Device class created with success");

	/*Create device file on /dev/ */
	if(device_create(myclass,NULL,major,NULL,"statusd_demo") == NULL){
		printk(KERN_ALERT "Failed to create device file");
                return -1;
	}
        printk("Device file created with success");

	/*Initialize device*/
	cdev_init(&statusd_dev,&statusd_fops);

	/*Associate device to the major number*/
	if(cdev_add(&statusd_dev,major,1)==-1){

		printk(KERN_ALERT "Failed to associate device");
		return -1;
	}

	
	return 0;
}


static void statusd_exit(void){

	/*Destroy device file*/
	device_destroy(myclass, major);
	cdev_del(&statusd_dev);

	/*Destroy device class*/
	class_destroy(myclass);

	/*Unregister character device*/
	unregister_chrdev(major,"statusd_demo");
	unregister_chrdev_region(major,1);
	
	printk(KERN_INFO "Statusd_demo stopped");
}


module_init(statusd_init);
module_exit(statusd_exit);

MODULE_AUTHOR("William Felipe Welter");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux kernel module that create a character device to simulate/force uniterruptble sleep state for testing purpose");
