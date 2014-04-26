#ifndef NETCAT_H_INCLUDED_
struct netcat_track {
	char *name;
	unsigned long len;
	char data[];
};


#endif /* NETCAT_H_INCLUDED_ */
