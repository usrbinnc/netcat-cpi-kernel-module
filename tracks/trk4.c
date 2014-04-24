#define NETCAT_CPI_TRK4_LEN 10788741
static char netcat_cpi_trk4[] = {

#include "trk4data.h"

};

static char *netcat_cpi_trk4_name = "netcat";

void netcat_cpi_register_trk4(char **trk, char **name, unsigned long *len)
{
	*trk = netcat_cpi_trk4;
	*name = netcat_cpi_trk4_name;
	*len = NETCAT_CPI_TRK4_LEN;
}
