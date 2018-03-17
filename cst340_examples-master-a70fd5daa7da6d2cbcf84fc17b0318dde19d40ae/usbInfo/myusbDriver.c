#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
 
// Defines
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>"); 
MODULE_DESCRIPTION("Sample USB Driver"); 

// Variables
static struct usb_device * device; 

// USB Probe Function
static int myusbProbe(struct usb_interface * interface,  const struct usb_device_id * id)
{
    // Variables
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;
 

    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "myusbDriver i/f %d now probed: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);
 
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;
 
        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize);
    }
 
    device = interface_to_usbdev(interface);
    return 0;
//    printk(KERN_INFO "myusb flash drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
//    return 0;
}

// USB disconnect function
static void myusbDisconnect(struct usb_interface * interface)
{
    printk(KERN_INFO "myusb flash drive %d now disconnected\n", 
            interface->cur_altsetting->desc.bInterfaceNumber);
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
