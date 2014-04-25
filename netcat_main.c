#define pr_fmt(fmt) "[" KBUILD_MODNAME "]: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

/* Brandon's compiler crashes unless we include them in
 * this order.
 *
 *   __                               __
 * _/  |_  ____   ____ _____    _____/  |_
 * \   __\/ __ \ /    \\__  \ _/ ___\   __\
 *  |  | \  ___/|   |  \/ __ \\  \___|  |
 *  |__|  \___  >___|  (____  /\___  >__|
 *            \/     \/     \/     \/
 */

#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */
#define NUM_TRACKS 6

struct netcat {
	char	*msg;
	bool	first_time;
	int	current_track;
};

/* TODO: Convert this a linked list to allow a module_init in each track to
 * register themselves
 */
static char *tracks[NUM_TRACKS] = {0, };
static char *tracknames[NUM_TRACKS] = {0, };
static unsigned long tracklens[NUM_TRACKS] = {0, };


/* TODO: Move these externs to a header file */
void netcat_cpi_register_trk1(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk2(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk3(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk4(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk5(char **trk, char **name, unsigned long *len);
void netcat_cpi_register_trk6(char **trk, char **name, unsigned long *len);


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
		if (current_track >= NUM_TRACKS)
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
