/*
 * =====================================================================================
 *
 *       Filename:  mychar.c
 *
 *    Description:  Simplified CHAR driver mychar
 *
 *        Version:  1.0
 *        Created:  05/12/2015 11:08:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Troy Scevers (troy.scevers@oit.edu)
 *   Organization:  Oregon Tech
 *
 * =====================================================================================
 */

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// MODULE Definitions
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>");
MODULE_DESCRIPTION("mychar CHAR Driver Example");

// Variables
static dev_t majorNum; 
static struct cdev CharDevice; 
static struct class * CharClass; 
static char myBuf = '\0'; 


// Operational Functions
static int mycharOpen(struct inode * i, struct file * f)
{
    printk(KERN_INFO "mychar Driver: Open Function\n"); 
    return 0; 
}

static int mycharClose(struct inode * i,  struct file * f)
{
    printk(KERN_INFO "mychar Driver: Close Function\n"); 
    return 0;
}

static ssize_t mycharRead(struct file * f, char __user * buf, 
        size_t len, loff_t * offset)
{
    printk(KERN_INFO "mychar Driver: Read Function\n"); 

	// Send my buffer to userland
	if(copy_to_user(buf, &myBuf, 1) != 0)
		return -EFAULT; 
	else
		return 1; 
}

static ssize_t mycharWrite(struct file * f, const char __user * buf, 
        size_t len, loff_t * offset)
{
    printk(KERN_INFO "mychar Driver: Write Function\n"); 

	// Copy last char from userland to my buffer
	if(copy_from_user(&myBuf, buf + len - 1, 1) != 0)
		return -EFAULT; 
	else
		return len; 
}

// Setup fops
static struct file_operations fops =
{
    .owner = THIS_MODULE, 
    .open = mycharOpen, 
    .release = mycharClose, 
    .read = mycharRead, 
    .write = mycharWrite
}; 

// Driver Init Function
static int __init mycharInit(void)
{
    printk(KERN_INFO "mychar Driver: Registered\n"); 
    
    // Attempt to Register Character device
    if(alloc_chrdev_region(&majorNum, 0, 1, "Troy") < 0)
    {
        return -1; 
    }

    // Attempt to create the device class for udev
    if((CharClass = class_create(THIS_MODULE, "chardrv"))  ==  NULL)
    {
        unregister_chrdev_region(majorNum, 1); 
        return -1; 
    }

    // Create the actual device in /dev folder
    if(device_create(CharClass, NULL, majorNum, NULL, "mychar")  ==  NULL)
    {
        class_destroy(CharClass); 
        unregister_chrdev_region(majorNum, 1); 
        return -1; 
    }

    // Initialize the CHAR device
    cdev_init(&CharDevice, &fops); 
    if(cdev_add(&CharDevice, majorNum, 1)  ==  -1)
    {
        device_destroy(CharClass, majorNum); 
        class_destroy(CharClass); 
        unregister_chrdev_region(majorNum, 1); 
        return -1; 
    }

    return 0; 
}

static void __exit mycharExit(void)
{
    cdev_del(&CharDevice); 
    device_destroy(CharClass, majorNum); 
    class_destroy(CharClass); 
    unregister_chrdev_region(majorNum, 1); 
    printk(KERN_INFO "mychar Driver: Unregistered\n"); 
}

module_init(mycharInit); 
module_exit(mycharExit); 

