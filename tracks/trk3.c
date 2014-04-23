#define NETCAT_CPI_TRK3_LEN 829971
static char netcat_cpi_trk3[] = {

#include <trk3data.h>

};

static char *netcat_cpi_trk3_name = "netcat";

void netcat_cpi_register_trk3(char **trk, char **name, unsigned long *len) {
    *trk = netcat_cpi_trk3;
    *name = netcat_cpi_trk3_name;
    *len = NETCAT_CPI_TRK3_LEN;
}
