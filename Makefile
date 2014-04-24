obj-m += netcat.o
EXTRA_CFLAGS:=-I$(src)/tracks 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
