#include <linux/bug.h>
#include <linux/kernel.h>
#include "../netcat.h"

static const char trk2_data[] = {
	#include "trk2data.h"
};

struct netcat_track netcat_cpi_trk2 = {
	.name = "The Internet is an Apt Motherfucker",
	.len  = ARRAY_SIZE(trk2_data),
	.data = trk2_data
};
