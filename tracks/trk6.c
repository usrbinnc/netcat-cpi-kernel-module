#define NETCAT_CPI_TRK6_LEN 13025371
static char netcat_cpi_trk6[] = {

#include <trk6data.h>

};

static char *netcat_cpi_trk6_name = "Approximating the Circumference of the Earth";

void netcat_cpi_register_trk6(char **trk, char **name, unsigned long *len) {
    *trk = netcat_cpi_trk6;
    *name = netcat_cpi_trk6_name;
    *len = NETCAT_CPI_TRK6_LEN;
}
