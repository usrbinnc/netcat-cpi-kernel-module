#include "netcat.h"
#define NETCAT_CPI_TRK1_LEN 624344
struct netcat_track netcat_cpi_trk1 = {
	.name = "Interrupt 0x7f",
	.len  = NETCAT_CPI_TRK1_LEN,
	.data = {
#include "trk1data.h"
	}
};
