netcat-objs += netcat_main.o
netcat-objs += tracks/trk1.o
netcat-objs += tracks/trk2.o
netcat-objs += tracks/trk3.o
netcat-objs += tracks/trk4.o
netcat-objs += tracks/trk5.o
netcat-objs += tracks/trk6.o
obj-m += netcat.o
EXTRA_CFLAGS=-I$(PWD)/tracks 
all:
	+$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	+$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
