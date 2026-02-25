obj-m += mychardev.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean