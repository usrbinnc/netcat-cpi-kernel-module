#define NETCAT_CPI_TRK5_LEN 858054
static char netcat_cpi_trk5[] = {

#include "trk5data.h"

};

static char *netcat_cpi_trk5_name = "Interrupt 0xbb";

void netcat_cpi_register_trk5(char **trk, char **name, unsigned long *len)
{
	*trk = netcat_cpi_trk5;
	*name = netcat_cpi_trk5_name;
	*len = NETCAT_CPI_TRK5_LEN;
}
