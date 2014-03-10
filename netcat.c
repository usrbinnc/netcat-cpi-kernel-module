
/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include "trk4.h"
#include "trk5.h"
#include "trk6.h"
#include "trk1.h"
#include "trk2.h"
#include "trk3.h"
/*  
 *  Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */
static char *msg_Ptr;

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

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

	printk(KERN_INFO "netcat - Cycles Per Instruction - Kernel Module Edition - 2014\nnetcat is Brandon Lucia, Andrew Olmstead, and David Balatero\nRun 'mknod /dev/netcat c %d 0' to setup the device.\ncat to ogg123 to play.\n", Major);

	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 
	 */
	unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	msg_Ptr = tracks[0];//track 1
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */

	/* 
	 * Decrement the usage count, or else once you opened the file, you'll
	 * never get get rid of the module. 
	 */
	module_put(THIS_MODULE);

	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	/*
	 * Number of bytes actually written to the buffer 
	 */
	int bytes_read = 0;

	/*
	 * If we're at the end of the message, 
	 * return 0 signifying end of file 
	 */
	if (msg_Ptr - tracks[currentTrack] >= tracklens[currentTrack]){

          /*End of Track.  Skip to next track, or finish if it's track 6*/ 
          currentTrack = (currentTrack + 1);
          if( currentTrack >= 6 ){ currentTrack = 0; }
	  printk(KERN_ALERT "[netcat]: Now playing track %d - %s\n",currentTrack + 1,tracknames[currentTrack]);
          msg_Ptr = tracks[currentTrack];

        }

	/* 
	 * Actually put the data into the buffer 
	 */
	while (length && msg_Ptr - tracks[currentTrack] < tracklens[currentTrack]) {

		/* 
		 * The buffer is in the user data segment, not the kernel 
		 * segment so "*" assignment won't work.  We have to use 
		 * put_user which copies data from the kernel data segment to
		 * the user data segment. 
		 */
		put_user(*(msg_Ptr++), buffer++);

		length--;
		bytes_read++;
	}

	/* 
	 * Most read functions return the number of bytes put into the buffer
	 */
	return bytes_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	printk(KERN_ALERT "[netcat]: Writing to netcat - Cycles Per Instruction isn't supported.\n");
	return -EINVAL;
}
