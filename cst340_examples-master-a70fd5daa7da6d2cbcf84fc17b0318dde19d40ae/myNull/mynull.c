/*
 * =====================================================================================
 *
 *       Filename:  mynull.c
 *
 *    Description:  Simplified CHAR driver mynull
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

// MODULE Definitions
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>");
MODULE_DESCRIPTION("mynull CHAR Driver Example");

// Variables
static dev_t majorNum; 
static struct cdev CharDevice; 
static struct class * CharClass; 


// Operational Functions
static int mynullOpen(struct inode * i, struct file * f)
{
    printk(KERN_INFO "mynull Driver: Open Function\n"); 
    return 0; 
}

static int mynullClose(struct inode * i,  struct file * f)
{
    printk(KERN_INFO "mynull Driver: Close Function\n"); 
    return 0;
}

static ssize_t mynullRead(struct file * f, char __user * buf, 
        size_t len, loff_t * offset)
{
    printk(KERN_INFO "mynull Driver: Read Function\n"); 
    return 0; 
}

static ssize_t mynullWrite(struct file * f, const char __user * buf, 
        size_t len, loff_t * offset)
{
    printk(KERN_INFO "mynull Driver: Write Function\n"); 
    return len; 
}

// Setup fops
static struct file_operations fops =
{
    .owner = THIS_MODULE, 
    .open = mynullOpen, 
    .release = mynullClose, 
    .read = mynullRead, 
    .write = mynullWrite
}; 

// Driver Init Function
static int __init mynullInit(void)
{
    printk(KERN_INFO "mynull Driver: Registered\n"); 
    
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
    if(device_create(CharClass, NULL, majorNum, NULL, "mynull")  ==  NULL)
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

static void __exit mynullExit(void)
{
    cdev_del(&CharDevice); 
    device_destroy(CharClass, majorNum); 
    class_destroy(CharClass); 
    unregister_chrdev_region(majorNum, 1); 
    printk(KERN_INFO "mynull Driver: Unregistered\n"); 
}

module_init(mynullInit); 
module_exit(mynullExit); 

