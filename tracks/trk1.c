#include <linux/bug.h>
#include <linux/kernel.h>
#include "../netcat.h"

static const char trk1_data[] = {
	#include "trk1data.h"
};

struct netcat_track netcat_cpi_trk1 = {
	.name = "Interrupt 0x7f",
	.len  = ARRAY_SIZE(trk1_data),
	.data = trk1_data
};
