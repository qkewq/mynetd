#ifndef PARSER_H
#define PARSER_H

#define DEFAULT_CONFIG_FILE "/etc/mynetd/mynetd.conf"
#define ADDRS_INHERIT NULL

// This is so scuffed
// Should separate these into headers

typedef enum services_t{
	SERV_MYNETD = 0x01,
	SERV_ECHO = 0x02,
	SERV_QOTD = 0x04,
	SERV_TIME = 0x08,
	SERV_DAYTIME = 0x10,
	SERV_CHARGEN = 0x20,
	SERV_DISCARD = 0x40,
} services_t;

typedef enum log_levels_t{
	LEVEL_INHERIT = 0,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
	LEVEL_FATAL,
} log_levels_t;

typedef enum rate_limits_t{
	LIMIT_INHERIT = 0,
	LIMIT_NONE,
	LIMIT_LIGHT,
	LIMIT_MEDIUM,
	LIMIT_STRICT,
} rate_limits_t;

typedef enum transport_rules_t{
	TP_TCP_INHERIT = 0x01,
	TP_TCP_ENABLE = 0x02,
	TP_UDP_INHERIT = 0x04,
	TP_UDP_ENABLE = 0x08,
} transport_rules_t;

typedef struct addrlist_t{
	struct addrlist_t *next;
	struct sockaddr_storage addr;
} addrlist_t;

typedef enum daytime_type_t{
	DTIME_NONE = 0,
	DTIME_ISO8601,
} daytime_type_t;

typedef struct service_config_t{
	struct service_config_t *next;
	enum services_t service;
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
	char *filepath;
	enum daytime_type_t time_type;
	uint16_t port;
} service_config_t;

// Server level configs

typedef struct configs_t{
	struct service_config_t *services;
	struct addrlist_t *addrs;
	enum log_levels_t log_level;
	enum rate_limits_t limit_level;
	enum transport_rules_t transport;
} configs_t;

int parse_config_file(char *path, configs_t **ret);
void free_configs(configs_t *configs);

#endif
