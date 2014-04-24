obj-m += netcat.o

KERNELDIR	?=  /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)

all: tracks.o
	$(RM) -f netcat.ko
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	mv netcat.ko netcat.ko.almost
	$(LD) -r -o netcat.ko netcat.ko.almost tracks.o

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

tracks.o: *.ogg
	$(LD) -r -b binary -o $@ $^
