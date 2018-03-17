/*
 * ========================================================================
 *
 *       Filename:  hello.c
 *
 *    Description:  Hello World Linux CHAR Driver (using UDEV)
 *
 *        Version:  1.0
 *        Created:  10/23/2012 10:02:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Troy Scevers (), troy.scevers@oit.edu
 *        Company:  Oregon Institute of Technology
 *
 * ========================================================================
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/*  Module Information */
MODULE_AUTHOR("Troy Scevers");
MODULE_DESCRIPTION("Hello World CHAR Device using UDEV");
MODULE_VERSION("1");
MODULE_LICENSE("Dual BSD/GPL");

/* Device Variables */
static struct class* hello_class = NULL;
static struct device* hello_device = NULL;
static int hello_major;
static int hello = 0;

/* Module Functions */
static int hello_open(struct inode * myInode, struct file * myFile)
{
    printk(KERN_INFO "Hello World Driver: Open Function!\n");
    return 0;
}

static int hello_close(struct inode * myInode, struct file * myFile)
{
    printk(KERN_INFO "Hello World Driver: Close Function!\n");
    return 0;
}

//int hello_read(void)
static ssize_t hello_read(struct file * file, char * buf, 
		size_t count, loff_t *ppos)
{
    char *strHello = "Hello, World!\n";
	int len = strlen(strHello);
    
    printk("%s:hello=%d\n",__FUNCTION__,hello++);

	if (count < len)
		return EINVAL;

	if (*ppos != 0)
		return 0;

	if (copy_to_user(buf, strHello, len))
		return EINVAL;

	*ppos = len;

	return len;
}

static struct file_operations fops = 
{
    .owner = THIS_MODULE, 
    .read = hello_read,
    .open = hello_open,
    .release = hello_close
};

static int __init hello_module_init(void)
{
    int retval;

    hello_major = register_chrdev(0, "device", &fops);
    if (hello_major < 0)
    {
        retval = hello_major;
        goto failed_chrdevreg;
    }

    hello_class = class_create(THIS_MODULE, "hello");
    if (IS_ERR(hello_class))
    {
        retval = PTR_ERR(hello_class);
        goto failed_classreg;
    }

    hello_device = device_create(hello_class, NULL, MKDEV(hello_major, 0), NULL, "hello_device");
    if (IS_ERR(hello_device))
    {
        retval = PTR_ERR(hello_device);
        goto failed_devreg;
    }

    printk(KERN_INFO "Hello World Driver: Driver Intialized!\n");
    return 0;

failed_devreg:
    class_unregister(hello_class);
    class_destroy(hello_class);
failed_classreg:
    unregister_chrdev(hello_major, "device");
failed_chrdevreg:
     return retval;
}

static void __exit hello_module_exit(void)
{
    device_destroy(hello_class, MKDEV(hello_major, 0));
    class_unregister(hello_class);
    class_destroy(hello_class);
    unregister_chrdev(hello_major, "device");

    printk(KERN_INFO "Hello World Driver: Driver Exited!\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);

