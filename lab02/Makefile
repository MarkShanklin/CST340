#if called directly from the command line, invoke the kernel build system.
ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default: module app

module:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) clean
	${RM} app

# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else

    obj-m := driver.o

endif