#define pr_fmt(fmt) "[" KBUILD_MODNAME "]: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>


#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */
#define NUM_TRACKS 6

static int Device_Open = 0;
static char *msg_Ptr = NULL;

static unsigned int firstTime = 1;
static unsigned int currentTrack = 0;

//TODO: Convert this a linked list to allow a module_init in each track to register themselves
static char *tracks[NUM_TRACKS] = {0, };
static char *tracknames[NUM_TRACKS] = {0, };
static unsigned long tracklens[NUM_TRACKS] = {0, };

void netcat_cpi_register_trk1(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk2(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk3(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk4(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk5(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk6(char **trk, char **name, unsigned long *len);

static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	msg_Ptr = tracks[0];	/* track 1 */

	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;
	if (Device_Open <= 0) {
		firstTime = 1;
	}

	return 0;
}

static ssize_t device_read(struct file *filp,
			   char *buffer,
			   size_t length,
			   loff_t *offset)
{
	int bytes_read = 0;

	if (firstTime == 1) {
		pr_info("Now playing track %d - %s\n",
			currentTrack + 1, tracknames[currentTrack]);
		firstTime = 0;
	}

	if (msg_Ptr - tracks[currentTrack] >= tracklens[currentTrack]) {
		/*End of Track.  Skip to next track, or finish if it's track 6*/
		currentTrack = (currentTrack + 1);
		if (currentTrack >= NUM_TRACKS)
			currentTrack = 0;
		pr_info("Now playing track %d - %s\n",
			currentTrack + 1, tracknames[currentTrack]);
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
device_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	pr_err("Writing to netcat - Cycles Per Instruction isn't supported.\n");
	return -EINVAL;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static struct miscdevice netcat_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fops,
	.mode = S_IRUGO,
};

static int netcat_init(void)
{
	int ret;
	int i = 0;

	ret = misc_register(&netcat_dev);
	if (ret) {
		pr_err("misc device register failed\n");
		return ret;
	}

	pr_info("netcat - Cycles Per Instruction - Kernel Module Edition - 2014\n");
	pr_info("netcat is Brandon Lucia, Andrew Olmstead, and David Balatero\n");
	pr_info("On the web at http://netcat.co\n");

	i = 0;
	pr_info("Loading track information...\n");
	netcat_cpi_register_trk1(&tracks[i], &tracknames[i], &tracklens[i]);
	i++;
	netcat_cpi_register_trk2(&tracks[i], &tracknames[i], &tracklens[i]);
	i++;
	netcat_cpi_register_trk3(&tracks[i], &tracknames[i], &tracklens[i]);
	i++;
	netcat_cpi_register_trk4(&tracks[i], &tracknames[i], &tracklens[i]);
	i++;
	netcat_cpi_register_trk5(&tracks[i], &tracknames[i], &tracklens[i]);
	i++;
	netcat_cpi_register_trk6(&tracks[i], &tracknames[i], &tracklens[i]);
	i++;
	BUG_ON(i != NUM_TRACKS);

	pr_info("'ogg123 - < /dev/netcat' to play.\n");

	return 0;
}

static void netcat_exit(void)
{
	misc_deregister(&netcat_dev);
}

module_init(netcat_init);
module_exit(netcat_exit);
