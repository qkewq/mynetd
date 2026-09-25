#ifndef PARSER_H
#define PARSER_H

#define DEFAULT_CONF_FILE "/etc/mynetd/mynetd.conf"
#define NUM_SERVICES 4

typedef struct addrlist_t{
	struct addrlist_t *next;
	struct sockaddr_storage addr;
} addrlist_t;

typedef struct configs_t{
	addrlist_t *addrs;
	void *service_confs[NUM_SERVICES];
} configs_t;


#endif
