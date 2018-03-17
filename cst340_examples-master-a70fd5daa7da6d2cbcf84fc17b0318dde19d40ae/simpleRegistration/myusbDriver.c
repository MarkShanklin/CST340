#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
 
// Defines
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>"); 
MODULE_DESCRIPTION("Sample USB Driver"); 

// USB Probe Function
static int myusbProbe(struct usb_interface * interface,  const struct usb_device_id * id)
{
    printk(KERN_INFO "myusb flash drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0;
}

// USB disconnect function
static void myusbDisconnect(struct usb_interface * interface)
{
    printk(KERN_INFO "myusb flash drive removed\n"); 
}

// Device ID struct
static struct usb_device_id myusbTable[] =
{
    //{ USB_DEVICE(0x2897, 0x5350) }, 
    { USB_DEVICE(0x090c, 0x1000) }, 
    {} // Terminating entry
}; 

// Register Device Table
MODULE_DEVICE_TABLE(usb, myusbTable); 

// USB Driver Struct
static struct usb_driver myusbDriver =
{
    .name = "myusbDriver", 
    .id_table = myusbTable, 
    .probe = myusbProbe, 
    .disconnect = myusbDisconnect, 
}; 

// Module Init Function
static int __init myusbInit(void)
{
    return usb_register(&myusbDriver); 
}

// Module Exit Function
static void __exit myusbExit(void)
{
    usb_deregister(&myusbDriver); 
}

module_init(myusbInit); 
module_exit(myusbExit); 
