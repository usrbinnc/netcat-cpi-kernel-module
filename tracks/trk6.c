#include "../netcat.h"
#define NETCAT_CPI_TRK6_LEN 13025371
struct netcat_track netcat_cpi_trk6 = {
	.name = "Approximating the Circumference of the Earth",
	.len  = NETCAT_CPI_TRK6_LEN,
	.data = {
#include "trk6data.h"
	}
};
