#include <linux/bug.h>
#include <linux/kernel.h>
#include "../netcat.h"

static const char trk3_data[] = {
	#include "trk3data.h"
};

struct netcat_track netcat_cpi_trk3 = {
	.name = "Interrupt 0x0d",
	.len  = ARRAY_SIZE(trk3_data),
	.data = trk3_data
};
