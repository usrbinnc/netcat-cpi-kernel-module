#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	
#include <linux/miscdevice.h>
// Brandon's compiler crashes unless we include them in
// this order.
//
//   __                               __   
// _/  |_  ____   ____ _____    _____/  |_ 
// \   __\/ __ \ /    \\__  \ _/ ___\   __\
//  |  | \  ___/|   |  \/ __ \\  \___|  |  
//  |__|  \___  >___|  (____  /\___  >__|  
//            \/     \/     \/     \/      
#include <trk4.h>
#include <trk5.h>
#include <trk6.h>
#include <trk1.h>
#include <trk2.h>
#include <trk3.h>

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */

static int Device_Open = 0;	
static char *msg_Ptr;

static unsigned int firstTime = 1;
static unsigned int currentTrack = 0;

static char *tracks[] = {netcat_cpi_trk1,
                         netcat_cpi_trk2,
                         netcat_cpi_trk3,
                         netcat_cpi_trk4,
                         netcat_cpi_trk5,
                         netcat_cpi_trk6};

static char *tracknames[] = {"Interrupt 0x7f",
                         "The Internet is an Apt Motherfucker",
                         "Interrupt 0x0d",
                         "netcat",
                         "Interrupt 0xbb",
                         "Approximating the Circumference of the Earth"};

static unsigned long tracklens[] = {NETCAT_CPI_TRK1_LEN,
                                    NETCAT_CPI_TRK2_LEN,
                                    NETCAT_CPI_TRK3_LEN,
                                    NETCAT_CPI_TRK4_LEN,
                                    NETCAT_CPI_TRK5_LEN,
                                    NETCAT_CPI_TRK6_LEN};


static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static struct miscdevice netcat_dev = {
	MISC_DYNAMIC_MINOR,
	DEVICE_NAME,
	&fops,
	};

int init_module(void)
{
	int ret;
	netcat_dev.mode=S_IRUGO;
	ret = misc_register(&netcat_dev);
	if (ret){
		printk(KERN_ERR "netcat: misc device register failed\n");
		goto out_noreg;
	}
	printk(KERN_INFO "[netcat]: netcat - Cycles Per Instruction - Kernel Module Edition - 2014\n[netcat]: netcat is Brandon Lucia, Andrew Olmstead, and David Balatero\n[netcat]: On the web at http://netcat.co\n[netcat]: 'cat /dev/netcat | ogg123 -' to play.\n");

	return SUCCESS;

	out_noreg:
	return ret;
}

void cleanup_module(void)
{
	misc_deregister(&netcat_dev);
}


static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	msg_Ptr = tracks[0];//track 1
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		

	module_put(THIS_MODULE);

	return 0;
}

static ssize_t device_read(struct file *filp,	
			   char *buffer,	
			   size_t length,	
			   loff_t * offset)
{
	int bytes_read = 0;

        if( firstTime == 1 ){

	  printk(KERN_ALERT "[netcat]: Now playing track %d - %s\n",currentTrack + 1,tracknames[currentTrack]);
          firstTime = 0;

        }

	if (msg_Ptr - tracks[currentTrack] >= tracklens[currentTrack]){

          /*End of Track.  Skip to next track, or finish if it's track 6*/ 
          currentTrack = (currentTrack + 1);
          if( currentTrack >= 6 ){ currentTrack = 0; }
	  printk(KERN_ALERT "[netcat]: Now playing track %d - %s\n",currentTrack + 1,tracknames[currentTrack]);
          msg_Ptr = tracks[currentTrack];

        }

	while (length && msg_Ptr - tracks[currentTrack] < tracklens[currentTrack]) {

		put_user(*(msg_Ptr++), buffer++);

		length--;
		bytes_read++;
	}

	return bytes_read;
}

static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	printk(KERN_ALERT "[netcat]: Writing to netcat - Cycles Per Instruction isn't supported.\n");
	return -EINVAL;
}
