This is a very simple Hello World driver module for Linux. this is about as simple as it can get. You must have build essentials installed and kernel header files installed on your system to build it.

use the following sequence to test.
make
    This should build the driver and you should get a bunch of intermediate files and a file called hello.ko

To insert the driver into the kernel use the following command
sudo insmod hello.ko

To remove the driver from the kernl use the following command
sudo rmmod hello

To see the messages from the driver
dmesg

To list all kernel modules 
lsmod
