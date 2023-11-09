obj-m += hello1.o hello2.o

# Set the ccflags-y option to include the inc directory
ccflags-y := -I$(src)/inc
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
