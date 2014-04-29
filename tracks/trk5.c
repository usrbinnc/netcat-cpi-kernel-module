#include <linux/bug.h>
#include <linux/kernel.h>
#include "../netcat.h"

static const char trk5_data[] = {
	#include "trk5data.h"
};

struct netcat_track netcat_cpi_trk5 = {
	.name = "Interrupt 0xbb",
	.len  = ARRAY_SIZE(trk5_data),
	.data = trk5_data
};
