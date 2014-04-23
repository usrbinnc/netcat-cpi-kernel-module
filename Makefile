netcat-objs += netcat_main.o
obj-m += netcat.o
KERNELDIR	?=  /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
