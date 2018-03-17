#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <asm/uaccess.h>

// Defines
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Troy Scevers <troy.scevers@oit.edu>"); 
MODULE_DESCRIPTION("Sample USB Driver"); 
#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN 0x82
#define MAX_PKT_SIZE 512

// Variables
static struct usb_device * device; 
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

// Control Functions
static int myusbOpen(struct inode * i, struct file * f)
{
    printk(KERN_INFO "myusb: Open Function Called\n");
    return 0; 
}

static int myusbClose(struct inode * i, struct file * f)
{
    printk(KERN_INFO "myusb: Close Function Called\n");
    return 0; 
}

static ssize_t myusbRead(struct file *f, char __user *buf, size_t cnt, loff_t *off)
{
    int retval;
    int read_cnt;

    printk(KERN_INFO "myusb: read function called\n");
 
    /* Read the data from the bulk endpoint */
    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN),
            bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
    if (copy_to_user(buf, bulk_buf, MIN(cnt, read_cnt)))
    {
        return -EFAULT;
    }
 
    return MIN(cnt, read_cnt);
}

static ssize_t myusbWrite(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{
    int retval;
    int wrote_cnt = MIN(cnt, MAX_PKT_SIZE);

    printk(KERN_INFO "myusb: write function called\n");
 
    if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE)))
    {
        return -EFAULT;
    }
 
    /* Write the data into the bulk endpoint */
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT),
            bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000);
    if (retval)
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
 
    return wrote_cnt;
}

static struct file_operations fops = 
{
    .open = myusbOpen, 
    .release = myusbClose, 
    .read = myusbRead, 
    .write = myusbWrite, 
}; 

// USB Probe Function
static int myusbProbe(struct usb_interface * interface,  const struct usb_device_id * id)
{
    // Variables
    int retval;
 
    device = interface_to_usbdev(interface);

    printk(KERN_INFO "myusb: Probing USB device\n");
 
    class.name = "usb/pen%d";
    class.fops = &fops;
    if ((retval = usb_register_dev(interface, &class)) < 0)
    {
        /* Something prevented us from registering this driver */
        printk(KERN_ERR "Not able to get a minor for this device.");
    }
    else
    {
        printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
    }
 
    return retval;
}

// USB disconnect function
static void myusbDisconnect(struct usb_interface * interface)
{
    printk(KERN_INFO "myusb: USB device disconnected\n");
    usb_deregister_dev(interface, &class);
}

// Device ID struct
static struct usb_device_id myusbTable[] =
{
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
    int result;
 
    /* Register this driver with the USB subsystem */
    if ((result = usb_register(&myusbDriver)))
    {
        printk(KERN_ERR "usb_register failed. Error number %d", result);
    }

    printk(KERN_INFO "myusb: driver initialized\n");

    return result;
}

// Module Exit Function
static void __exit myusbExit(void)
{
    usb_deregister(&myusbDriver);

   printk(KERN_INFO "myusb: Driver removed\n"); 
}

module_init(myusbInit); 
module_exit(myusbExit); 
