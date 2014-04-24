#define NETCAT_CPI_TRK1_LEN 624344
static char netcat_cpi_trk1[] = {

#include "trk1data.h"

};

static char *netcat_cpi_trk1_name = "Interrupt 0x7f";

void netcat_cpi_register_trk1(char **trk, char **name, unsigned long *len)
{
	*trk = netcat_cpi_trk1;
	*name = netcat_cpi_trk1_name;
	*len = NETCAT_CPI_TRK1_LEN;
}
