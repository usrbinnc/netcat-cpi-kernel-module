netcat-objs += netcat_main.o
netcat-objs += tracks/trk1.o
netcat-objs += tracks/trk2.o
netcat-objs += tracks/trk3.o
netcat-objs += tracks/trk4.o
netcat-objs += tracks/trk5.o
netcat-objs += tracks/trk6.o
obj-m += netcat.o
KERNELDIR	?=  /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
