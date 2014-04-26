#include "../netcat.h"
#define NETCAT_CPI_TRK5_LEN 858054
struct netcat_track netcat_cpi_trk5 = {
	.name = "Interrupt 0xbb",
	.len  = NETCAT_CPI_TRK5_LEN,
	.data = {
#include "trk5data.h"
	}
};
