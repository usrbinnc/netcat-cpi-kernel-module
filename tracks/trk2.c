#include "netcat.h"
#define NETCAT_CPI_TRK2_LEN 11659615
struct netcat_track netcat_cpi_trk2 = {
	.name = "The Internet is an Apt Motherfucker",
	.len  = NETCAT_CPI_TRK2_LEN,
	.data = {
#include "trk2data.h"
	}
};
