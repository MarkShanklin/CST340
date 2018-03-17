/*==========================================================================
 *       Filename:  hello.c
 *
 *    Description:  Hello World Driver
 *
 *        Version:  1.0
 *        Created:  01/09/2018 10:28:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Troy Scevers (troy.scevers@oit.edu)
 *   Organization:  Oregon Institute of Technology
 *
 *==========================================================================
 */

#include <linux/init.h>
#include <linux/module.h>

// Declare The Modules' Information
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>"); 
MODULE_DESCRIPTION("Hello World Driver Module");

//Init function for the driver
static int __init hello_init(void)
{
    printk(KERN_ALERT "Hello CST 340\n");
    return 0;
}

//Driver Exit Function
static void __exit hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, Linux World!!!\n");
}

//Register the init and exit functions with the module interface
module_init(hello_init);
module_exit(hello_exit);
