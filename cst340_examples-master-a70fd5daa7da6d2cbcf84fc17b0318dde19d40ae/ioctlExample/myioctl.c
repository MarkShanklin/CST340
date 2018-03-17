#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
 
#include "myioctl.h"
 
// Module Defines
#define FIRST_MINOR 0
#define MINOR_CNT 1
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>");
MODULE_DESCRIPTION("ioctl() Example Char Driver");

// Module Variables
static dev_t dev; 
static struct cdev c_dev; 
static struct class *cl; 
static int status = 1; 
static int dignity = 3; 
static int ego = 5; 

static int myioctlOpen(struct inode * i, struct file * f)
{
    printk(KERN_INFO "myioctl driver: Open function called\n"); 
    return 0; 
}

static int myioctlClose(struct inode * i,  struct file * f)
{
    printk(KERN_INFO "myioctl driver: Close function called\n"); 
    return 0; 
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    query_arg_t q; 

    printk(KERN_INFO "myioctl driver: ioctl function called\n"); 

    switch(cmd)
    {
        case QUERY_GET_VARIABLES:
            q.status = status;
            q.dignity = dignity;
            q.ego = ego;
            if (copy_to_user((query_arg_t *)arg, &q, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            break;
        case QUERY_CLR_VARIABLES:
            status = 0;
            dignity = 0;
            ego = 0;
            break;
        case QUERY_SET_VARIABLES:
            if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            status = q.status;
            dignity = q.dignity;
            ego = q.ego;
            break;
        default:
            return -EINVAL;
    }
    return 0; 
}

static struct file_operations fops =
{
    .owner = THIS_MODULE, 
    .open = myioctlOpen, 
    .release = myioctlClose, 
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = myioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
}; 

static int __init myioctlInit(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_ioctl")) < 0)
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

    printk(KERN_INFO "myioctl driver: Loaded\n"); 
 
    return 0;
}

static void __exit myioctlExit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);

    printk(KERN_INFO "myioctl driver: unloaded\n"); 
}

module_init(myioctlInit); 
module_exit(myioctlExit); 
