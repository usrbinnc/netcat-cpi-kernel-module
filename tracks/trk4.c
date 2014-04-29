#include <linux/bug.h>
#include <linux/kernel.h>
#include "../netcat.h"

static const char trk4_data[] = {
	#include "trk4data.h"
};

struct netcat_track netcat_cpi_trk4 = {
	.name = "netcat",
	.len  = ARRAY_SIZE(trk4_data),
	.data = trk4_data
};
