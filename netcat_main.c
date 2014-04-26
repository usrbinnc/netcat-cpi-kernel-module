#define pr_fmt(fmt) "[" KBUILD_MODNAME "]: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#include "netcat.h"

struct netcat {
	char	*msg;
	bool	first_time;
	int	current_track;
};

#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */

extern struct netcat_track netcat_cpi_trk1;
extern struct netcat_track netcat_cpi_trk2;
extern struct netcat_track netcat_cpi_trk3;
extern struct netcat_track netcat_cpi_trk4;
extern struct netcat_track netcat_cpi_trk5;
extern struct netcat_track netcat_cpi_trk6;

static struct netcat_track *tracks[] = {&netcat_cpi_trk1,
					&netcat_cpi_trk2,
					&netcat_cpi_trk3,
					&netcat_cpi_trk4,
					&netcat_cpi_trk5,
					&netcat_cpi_trk6};

static int device_open(struct inode *inode, struct file *file)
{
	struct netcat *netcat;

	netcat = kzalloc(sizeof(*netcat), GFP_KERNEL);
	if (!netcat)
		return -ENOMEM;

	netcat->first_time = true;
	netcat->msg = tracks[0]->data;	/* track 1 */
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
			current_track + 1, tracks[current_track]->name);
		netcat->first_time = false;
	}

	if (netcat->msg - tracks[current_track]->data >=
		tracks[current_track]->len) {
		/* End of Track.  Skip to next track, or finish if it's the last track */
		current_track++;
		if (current_track >= ARRAY_SIZE(tracks))
			current_track = 0;
		pr_info("Now playing track %d - %s\n",
			current_track + 1, tracks[current_track]->name);
		netcat->msg = tracks[current_track]->data;
		netcat->current_track = current_track;
	}

	while (length &&
		(netcat->msg - tracks[current_track]->data) <
		 tracks[current_track]->len) {
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
