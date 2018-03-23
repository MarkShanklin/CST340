/***********************************************************
* File Name: driver.c
* Purpose: lab2 and final project driver
* Creation Date: 03/21/18
* Last Modified: 03/21/18
* Created By: Mark Shanklin
**********************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
 
#include "driver.h"

// Module Defines
#define FIRST_MINOR 0
#define MINOR_CNT 1
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Shanklin <mark.shanklin@oit.edu>");
MODULE_DESCRIPTION("lab2 and final project driver");

static dev_t dev; 
static struct cdev c_dev; 
static struct class *cl; 
static int leftArg = 1; 
static int rightArg = 3; 
static int answer = 5; 
static char buffer[512] = { 0 };

static int my_driver_Open(struct inode * i, struct file * f)
{
    printk(KERN_INFO "my_driver: Open function called\n"); 
    return 0; 
}

static int my_driver_Close(struct inode * i,  struct file * f)
{
    printk(KERN_INFO "my_driver: Close function called\n"); 
    return 0; 
}


static ssize_t my_driver_Read(struct file * f, char __user * buf,
        size_t len, loff_t * offset){
    printk(KERN_INFO "my_driver: Read Function\n");
 

    if(*offset  ==  0){
        if(copy_to_user(buf, buffer, strlen(buffer)) != 0)
            return -EFAULT;
        else{
            (*offset)++;
            return strlen(buffer);
        }
    }
    else
        return 0;
}

static ssize_t my_driver_Write(struct file *f, const char __user *buf, size_t len,     loff_t *offset)
{
    printk(KERN_INFO "my_driver: Write Function called!\n");
    
    size_t copy_length=len;
    if(len > 511) {
        copy_length = 511;
    }

    if(copy_from_user(&buffer, buf, copy_length) != 0)
        return -EFAULT;
    else {
        return len;
    }    

return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int mydriver(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_driver(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    query_arg_t q; 

    printk(KERN_INFO "my_driver: function called\n"); 

    switch(cmd)
    {
        case QUERY_GET_VARIABLES:
            q.leftArg = leftArg;
            q.rightArg = rightArg;
            q.answer = answer;
            if (copy_to_user((query_arg_t *)arg, &q, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            break;
        case QUERY_CLR_VARIABLES:
            leftArg = 0;
            rightArg = 0;
            answer = 0;
            break;
        case QUERY_SET_VARIABLES:
            if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            leftArg = q.leftArg;
            rightArg = q.rightArg;
            strcpy(buffer, q.buffer);
        case QUERY_NULL_FILL:
            memset(buffer, 0, 512);
            break;
        case QUERY_A_FILL:
            printk(KERN_INFO "QUERY_A_FILL");
            memset(buffer, 'A', 511);
            buffer[511] = 0;
            break;
        case QUERY_STATUS_BUFFER:
            if(buffer[0] != 0)
                return 1;
            else return 0;
        default:
            return -EINVAL;
    }
    return 0; 
}

static struct file_operations fops =
{
    .owner = THIS_MODULE, 
    .open = my_driver_Open, 
    .release = my_driver_Close,
    .read = my_driver_Read,
    .write = my_driver_Write, 
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = mydriver
#else
    .unlocked_ioctl = my_driver
#endif
};

static int __init my_driver_Init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_my_driver")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    printk(KERN_INFO "my_driver: Loaded\n"); 
 
    return 0;
}
 
static void __exit my_driver_Exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);

    printk(KERN_INFO "my_driver: unloaded\n"); 
}

module_init(my_driver_Init); 
module_exit(my_driver_Exit); 