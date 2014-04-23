netcat-objs += netcat_main.o
obj-m += netcat.o
EXTRA_CFLAGS=-I$(PWD)/tracks 
all:
	+$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	+$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
