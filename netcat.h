#ifndef NETCAT_H_INCLUDED_
struct netcat_track {
	char *name;
	unsigned long len;
	char data[];
};

extern struct netcat_track netcat_cpi_trk1;
extern struct netcat_track netcat_cpi_trk2;
extern struct netcat_track netcat_cpi_trk3;
extern struct netcat_track netcat_cpi_trk4;
extern struct netcat_track netcat_cpi_trk5;
extern struct netcat_track netcat_cpi_trk6;

#endif /* NETCAT_H_INCLUDED_ */
