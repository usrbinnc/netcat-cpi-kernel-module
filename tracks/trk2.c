#define NETCAT_CPI_TRK2_LEN 11659615
static char netcat_cpi_trk2[] = {

#include <trk2data.h>

};

static char *netcat_cpi_trk2_name = "The Internet is an Apt Motherfucker";

void netcat_cpi_register_trk2(char **trk, char **name, unsigned long *len) {
    *trk = netcat_cpi_trk2;
    *name = netcat_cpi_trk2_name;
    *len = NETCAT_CPI_TRK2_LEN;
}
