#include "netcat.h"
#define NETCAT_CPI_TRK3_LEN 829971
struct netcat_track netcat_cpi_trk3 = {
	.name = "Interrupt 0x0d",
	.len  = NETCAT_CPI_TRK3_LEN,
	.data = {
#include "trk3data.h"
	}
};
