#define pr_fmt(fmt) "[" KBUILD_MODNAME "]: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

 /*
 *   __                               __
 * _/  |_  ____   ____ _____    _____/  |_
 * \   __\/ __ \ /    \\__  \ _/ ___\   __\
 *  |  | \  ___/|   |  \/ __ \\  \___|  |
 *  |__|  \___  >___|  (____  /\___  >__|
 *            \/     \/     \/     \/
 */

#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */

struct netcat {
	char	*msg;
	bool	first_time;
	int	current_track;
};

extern char _binary_trk1_ogg_start;
extern char _binary_trk2_ogg_start;
extern char _binary_trk3_ogg_start;
extern char _binary_trk4_ogg_start;
extern char _binary_trk5_ogg_start;
extern char _binary_trk6_ogg_start;

static char *tracks[] = {&_binary_trk1_ogg_start,
			 &_binary_trk2_ogg_start,
			 &_binary_trk3_ogg_start,
			 &_binary_trk4_ogg_start,
			 &_binary_trk5_ogg_start,
			 &_binary_trk6_ogg_start};

static char *tracknames[] = {"Interrupt 0x7f",
			 "The Internet is an Apt Motherfucker",
			 "Interrupt 0x0d",
			 "netcat",
			 "Interrupt 0xbb",
			 "Approximating the Circumference of the Earth"};

extern char _binary_trk1_ogg_size;
extern char _binary_trk2_ogg_size;
extern char _binary_trk3_ogg_size;
extern char _binary_trk4_ogg_size;
extern char _binary_trk5_ogg_size;
extern char _binary_trk6_ogg_size;

#define CASTUL(_x) ((unsigned long)&(_x))
static unsigned long tracklens[] = {CASTUL(_binary_trk1_ogg_size),
				    CASTUL(_binary_trk2_ogg_size),
				    CASTUL(_binary_trk3_ogg_size),
				    CASTUL(_binary_trk4_ogg_size),
				    CASTUL(_binary_trk5_ogg_size),
				    CASTUL(_binary_trk6_ogg_size)};


static int device_open(struct inode *inode, struct file *file)
{
	struct netcat *netcat;

	netcat = kzalloc(sizeof(*netcat), GFP_KERNEL);
	if (!netcat)
		return -ENOMEM;

	netcat->first_time = true;
	netcat->msg = tracks[0];	/* track 1 */
	file->private_data = netcat;
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	struct netcat *netcat = file->private_data;

	kfree(netcat);
	return 0;
}

static ssize_t device_read(struct file *file,
			   char *buffer,
			   size_t length,
			   loff_t *offset)
{
	struct netcat *netcat = file->private_data;
	int current_track = netcat->current_track;

	int bytes_read = 0;

	if (netcat->first_time == true) {
		pr_info("Now playing track %d - %s\n",
			current_track + 1, tracknames[current_track]);
		netcat->first_time = false;
	}

	if (netcat->msg - tracks[current_track] >= tracklens[current_track]) {
		/* End of Track.  Skip to next track, or finish if it's track 6 */
		current_track++;
		if (current_track >= 6)
			current_track = 0;
		pr_info("Now playing track %d - %s\n",
			current_track + 1, tracknames[current_track]);
		netcat->msg = tracks[current_track];
		netcat->current_track = current_track;
	}

	while (length &&
		(netcat->msg - tracks[current_track]) <
		 tracklens[current_track]) {
		put_user(*(netcat->msg++), buffer++);

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

	ret = misc_register(&netcat_dev);
	if (ret) {
		pr_err("misc device register failed\n");
		return ret;
	}
	pr_info("netcat - Cycles Per Instruction - Kernel Module Edition - 2014\n");
	pr_info("netcat is Brandon Lucia, Andrew Olmstead, and David Balatero\n");
	pr_info("On the web at http://netcat.co\n");
	pr_info("'ogg123 - < /dev/netcat' to play.\n");

	return 0;
}

static void netcat_exit(void)
{
	misc_deregister(&netcat_dev);
}

module_init(netcat_init);
module_exit(netcat_exit);
