#include <linux/bug.h>
#include <linux/kernel.h>
#include "../netcat.h"

static const char trk6_data[] = {
	#include "trk6data.h"
};

struct netcat_track netcat_cpi_trk6 = {
	.name = "Approximating the Circumference of the Earth",
	.len  = ARRAY_SIZE(trk6_data),
	.data = trk6_data
};
