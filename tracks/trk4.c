#include "../netcat.h"
#define NETCAT_CPI_TRK4_LEN 10788741
struct netcat_track netcat_cpi_trk4 = {
	.name = "netcat",
	.len  = NETCAT_CPI_TRK4_LEN,
	.data = {
#include "trk4data.h"
	}
};
