#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/conf.h>   /* cdevsw struct */
#include <sys/types.h>
#include <sys/uio.h>

#include "netcat.h"

/* Function prototypes */
static d_open_t      netcat_open;
static d_close_t     netcat_close;
static d_read_t      netcat_read;

static struct cdevsw netcat_cdevsw = {
	.d_version = D_VERSION,
	.d_open = netcat_open,
	.d_close = netcat_close,
	.d_read = netcat_read,
	.d_name = "netcat",
	.d_flags = D_DISK
};

static struct netcat_track *tracks[] = {&netcat_cpi_trk1,
					&netcat_cpi_trk2,
					&netcat_cpi_trk3,
					&netcat_cpi_trk4,
					&netcat_cpi_trk5,
					&netcat_cpi_trk6};

static struct {
	int	current_track;
	size_t	bytes;
	bool	first;
} netcatdata;

static struct cdev *netcat_dev;

static int netcat_loader(struct module *module, int event, void *arg) {
	int e = 0;

	switch (event) {
		case MOD_LOAD:
			printf("netcat - Cycles Per Instruction - Kernel Module Edition - 2014\n");
			printf("netcat is Brandon Lucia, Andrew Olmstead, and David Balatero\n");
			printf("On the web at http://netcat.co\n");
			printf("'ogg123 - < /dev/netcat' to play.\n");
			e = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK,
				&netcat_dev,
				&netcat_cdevsw,
				0,
				UID_ROOT,
				GID_WHEEL,
				0444,
				"netcat");
			if (e != 0)
				break;

			break;
		case MOD_UNLOAD:
			destroy_dev(netcat_dev);
			break;

		default:
			e = EOPNOTSUPP;
			break;
	}

	return (e);
}

static int netcat_open(struct cdev *dev __unused, int oflags __unused, int devtype __unused, struct thread *td __unused)
{
	int error = 0;

	netcatdata.current_track = 0;
	netcatdata.bytes = 0;
	netcatdata.first = true;

	return (error);
}

static int netcat_close(struct cdev *dev __unused, int oflags __unused, int devtype __unused, struct thread *td __unused)
{
	int error = 0;

	return (error);
}

static int
netcat_read(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
	int error = 0;
	int totalbytesleft;

	totalbytesleft = uio->uio_resid;

	while (totalbytesleft > 0) {
		if (netcatdata.first) {
			printf("Now playing %s\n", tracks[netcatdata.current_track]->name);
			netcatdata.first = false;
		}

		int amt = MIN(totalbytesleft,
			tracks[netcatdata.current_track]->len - netcatdata.bytes);

		error = uiomove(tracks[netcatdata.current_track]->data +
			netcatdata.bytes, amt, uio);
		if (error != 0)
			return (error);

		netcatdata.bytes += amt;
		totalbytesleft -= amt;
		if (netcatdata.bytes == tracks[netcatdata.current_track]->len) {
			netcatdata.bytes = 0;
			netcatdata.first = true;
			netcatdata.current_track = (netcatdata.current_track + 1) % nitems(tracks);
		}

	}

	return (error);
}

DEV_MODULE(netcat, netcat_loader, NULL);

